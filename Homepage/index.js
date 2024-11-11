const { spawn } = require('child_process');
const express = require('express');
const path = require('path');

const app = express();
const port = 3000;

const projects = [
  { name: 'OpenGL Animation', type: 'cpp', makePath: '../OpenGL', executable: './sample', port: 4000 },
  { name: 'Exercise Tracker App', path: '/Users/mileskesser/Desktop/CS406-main/exercise-app/backend/server.js', port: 5002 },
  { name: 'Rock Paper Scissors Game', url: '/rock-paper-scissors' },
  { name: 'Figma Example', url: 'https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496' },
  { name: 'Weather App', path: '/Users/mileskesser/Desktop/dynamic-portfolio/weatherApp/backend/server.js', port: 5008 }
];

app.use(express.static(path.join(__dirname, '../')));

app.get('/rock-paper-scissors', (req, res) => {
  res.sendFile(path.join(__dirname, '../rock_paper_scissors.html'));  
});

app.get('/run-final', (req, res) => {
  const project = projects.find(p => p.name === 'OpenGL Animation');

  if (project) {
    console.log(`Building and running ${project.name}...`);

    const makeProcess = spawn('make', [], { cwd: path.resolve(__dirname, project.makePath) });

    makeProcess.stdout.on('data', (data) => {
      console.log(`${project.name} build output: ${data}`);
    });

    makeProcess.stderr.on('data', (data) => {
      console.error(`${project.name} build error: ${data}`);
    });

    makeProcess.on('close', (code) => {
      if (code === 0) {
        console.log(`${project.name} built successfully. Running executable...`);

        const executableProcess = spawn(project.executable, [], { cwd: path.resolve(__dirname, project.makePath) });

        executableProcess.stdout.on('data', (data) => {
          console.log(`${project.name} output: ${data}`);
        });

        executableProcess.stderr.on('data', (data) => {
          console.error(`${project.name} error: ${data}`);
        });

        executableProcess.on('close', (code) => {
          console.log(`${project.name} process exited with code ${code}`);
        });

        res.send(`<h1>${project.name} project is running and will launch momentarily! You can click and drag to interact with the animation</h1>`);
      } else {
        res.send(`<h1>${project.name} failed to build with code ${code}</h1>`);
      }
    });
  } else {
    res.send('<h1>Project not found!</h1>');
  }
});

// New route to build and run the Android Project (A3) in an emulator
app.get('/run-android', (req, res) => {
  const projectPath = '/Users/mileskesser/Desktop/dynamic-portfolio/A3';
  const emulatorName = 'Pixel 8 Pro API 35';
  const javaHomePath = '/Library/Java/JavaVirtualMachines/jdk-17.0.1.jdk/Contents/Home'; // Replace with the correct path


  console.log(`Building Android project at ${projectPath}...`);

  // Step 1: Build the project with Gradle
  const gradleBuild = spawn('./gradlew', ['assembleDebug'], {
    cwd: projectPath,
    shell: true,
    env: { ...process.env, JAVA_HOME: javaHomePath } // Use actual Java path
  });

  gradleBuild.stdout.on('data', (data) => {
    console.log(`Gradle build output: ${data}`);
  });

  gradleBuild.stderr.on('data', (data) => {
    console.error(`Gradle build error: ${data}`);
  });

  gradleBuild.on('close', (code) => {
    if (code === 0) {
      console.log('Build successful, launching emulator...');

      // Step 2: Start the emulator
      const emulatorProcess = spawn('emulator', ['-avd', emulatorName], {
        detached: true,
        stdio: 'ignore'
      });

      emulatorProcess.unref();

      setTimeout(() => {
        // Step 3: Install and run the APK in the emulator
        const apkPath = path.join(projectPath, 'app/build/outputs/apk/debug/app-debug.apk');
        const installProcess = spawn('adb', ['-s', 'emulator-5554', 'install', '-r', apkPath]);

        installProcess.stdout.on('data', (data) => {
          console.log(`Install output: ${data}`);
        });

        installProcess.stderr.on('data', (data) => {
          console.error(`Install error: ${data}`);
        });

        installProcess.on('close', (code) => {
          if (code === 0) {
            console.log('APK installed, launching the app...');

            const launchProcess = spawn('adb', ['-s', 'emulator-5554', 'shell', 'monkey', '-p', 'com.example.a3', '-c', 'android.intent.category.LAUNCHER', '1']);

            launchProcess.stdout.on('data', (data) => {
              console.log(`Launch output: ${data}`);
            });

            launchProcess.stderr.on('data', (data) => {
              console.error(`Launch error: ${data}`);
            });

            launchProcess.on('close', (code) => {
              if (code === 0) {
                console.log('App launched successfully!');
                res.send('<h1>Android Project A3 is running in the emulator!</h1>');
              } else {
                res.send('<h1>Failed to launch the app in the emulator.</h1>');
              }
            });
          } else {
            res.send('<h1>Failed to install the APK in the emulator.</h1>');
          }
        });
      }, 15000); // Wait for the emulator to boot up before installing APK
    } else {
      res.send('<h1>Gradle build failed.</h1>');
    }
  });
});

// Route to directly launch the Weather App on port 5008
app.get('/run-weather-app', (req, res) => {
  const project = projects.find(p => p.name === 'Weather App');
  if (project) {
    console.log(`Starting ${project.name} on port ${project.port}...`);
    const weatherProcess = spawn('node', [project.path]);

    weatherProcess.stdout.on('data', (data) => console.log(`${project.name} output: ${data}`));
    weatherProcess.stderr.on('data', (data) => console.error(`${project.name} error: ${data}`));

    weatherProcess.on('close', (code) => console.log(`${project.name} process exited with code ${code}`));
    res.redirect(`http://localhost:${project.port}/weather`);
  } else {
    res.send('<h1>Weather App project not found!</h1>');
  }
});


projects.forEach((project) => {
  if (project.path && project.type !== 'cpp') {
    const process = spawn('node', [project.path]);

    process.stdout.on('data', (data) => {
      console.log(`${project.name} running on port ${project.port}: ${data}`);
    });

    process.stderr.on('data', (data) => {
      console.error(`${project.name} error: ${data}`);
    });

    process.on('close', (code) => {
      console.log(`${project.name} process exited with code ${code}`);
    });
  }
});

app.get('/', (req, res) => {
  res.send(`
    <html>
      <head>
        <style>
          body, html {
            height: 100%;
            margin: 0;
            font-family: 'Helvetica Neue', Arial, sans-serif;
            background-color: #1c1c1e;
            color: #e0e0e0;
          }
          .container {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            grid-gap: 20px;
            padding: 60px;
            max-width: 1200px;
            margin: auto;
          }
          .quadrant {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            color: #ffffff;
            text-align: center;
            text-decoration: none;
            border-radius: 15px;
            padding: 30px;
            background: #333333;
            box-shadow: 0px 6px 12px rgba(0, 0, 0, 0.4);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
            height: 300px;
            position: relative;
            border: 1px solid #444444;
          }
          .quadrant:hover {
            transform: translateY(-8px);
            box-shadow: 0px 12px 20px rgba(0, 0, 0, 0.6);
          }
          .title {
            position: absolute;
            top: 20px;
            font-size: 20px;
            font-weight: 600;
            color: #ffffff;
            width: 100%;
            text-align: center;
          }
          #q1 { background-color: #007bff; }
          #q2 { background-color: #dc3545; }
          #q3 { background-color: #6f42c1; }
          #q4 { background-color: #28a745; }
          #q5 { background-color: #ff8c42; }
          .quadrant img {
            width: 70%;
            height: 60%;
            object-fit: contain;
            margin-top: 20px;
          }
          .quadrant p {
            font-size: 14px;
            color: #cccccc;
            margin-top: 15px;
          }
          h1 {
            text-align: center;
            font-size: 36px;
            color: #ffffff;
            margin-bottom: 40px;
            padding-top: 20px;
          }
        </style>
        <title>Dark-Themed Project Portfolio</title>
      </head>
      <body>
        <h1>Project Portfolio</h1>
        <div class="container">
          <a href="http://localhost:5002" id="q1" class="quadrant">
            <div class="title">Exercise Tracker API</div>
            <img src="www.png" alt="Exercise Tracker Image">
            <p>A powerful tool for tracking exercise progress and routines.</p>
          </a>
          <a href="http://localhost:3000/run-final" id="q2" class="quadrant">
            <div class="title">OpenGL Graphics</div>
            <img src="opengl.png" alt="OpenGL Animation Image">
            <p>High-performance animations rendered with OpenGL.</p>
          </a>
          <a href="/rock-paper-scissors" id="q3" class="quadrant">
            <div class="title">Rock Paper Scissors Game</div>
            <img src="RPS.png" alt="Rock Paper Scissors Image">
            <p>Classic game recreated with modern, interactive UI.</p>
          </a>
          <a href="https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496" id="q4" class="quadrant">
            <div class="title">Figma</div>
            <img src="figma.png" alt="Figma Example Image">
            <p>Prototyping tool to create and showcase design projects.</p>
          </a>
          <a href="http://localhost:3000/run-weather-app" id="q5" class="quadrant">
            <div class="title">Weather App</div>
            <p>Displays weather with dynamic city backgrounds.</p>
          </a>
          <a href="http://localhost:3000/run-android" id="q6" class="quadrant">
            <div class="title">Android Studio Project</div>
            <p>Comprehensive Android app development and launch environment.</p>
          </a>
        </div>
      </body>
    </html>
  `);
});

app.listen(port, () => {
  console.log(`\nPortfolio homepage running at http://localhost:${port}\n`);
});
