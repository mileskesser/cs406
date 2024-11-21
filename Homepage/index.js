const { spawn } = require('child_process');
const express = require('express');
const path = require('path');

const app = express();
const port = 3000;

const projects = [
  { name: 'OpenGL Animation', type: 'cpp', makePath: '../OpenGL', executable: './sample', port: 4000 },
  { name: 'Exercise Tracker App', path: '/Users/mileskesser/Desktop/dynamic-portfolio/exercise-app/backend/server.js', port: 5002 },
  { name: 'Rock Paper Scissors Game', url: '/rock-paper-scissors' },
  { name: 'Figma Example', url: 'https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496' },
  { name: 'Weather App', path: '/Users/mileskesser/Desktop/dynamic-portfolio/weatherApp/backend/server.js', port: 5008 },
  { name: 'Game', url: '/game' }
];

app.use(express.static(path.join(__dirname, '../')));

app.get('/rock-paper-scissors', (req, res) => {
  res.sendFile(path.join(__dirname, '../rock_paper_scissors.html'));  
});

app.get('/game', (req, res) => {
  res.sendFile(path.join(__dirname, '../game.html'));
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

// Route to serve the video player page
app.get('/play-video', (req, res) => {
  res.send(`
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Video Player</title>
      <style>
        body {
          display: flex;
          justify-content: center;
          align-items: center;
          height: 70%;
          margin: 0;
          background-color: #1c1c1e;
          color: #e0e0e0;
          font-family: Arial, sans-serif;
        }
        .video-container {
          text-align: center;
        }
        video {
          
          max-width: 90%;
          max-height: 90%;
          border-radius: 8px;
          box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.5);
        }
      </style>
    </head>
    <body>
      <div class="video-container">
        <h2>Click the video below to watch</h2>
        <video controls>
          <source src="/video.mov" type="video/mp4">
          Your browser does not support the video tag.
        </video>
      </div>
    </body>
    </html>
  `);
});

// Serve the video file directly
app.get('/video.mov', (req, res) => {
  const videoPath = path.join(__dirname, 'video.mov');
  res.sendFile(videoPath, (err) => {
    if (err) {
      console.error("Error serving video file:", err);
      res.status(404).send("Video file not found");
    } else {
      console.log("Serving video file at /video.mov");
    }
  });
});



app.get('/', (req, res) => {
  res.send(`
  <!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dynamic Portfolio</title>
    <style>
      /* Base Styles */
      body, html {
        height: 100%;
        margin: 0;
        font-family: 'Helvetica Neue', Arial, sans-serif;
        transition: background-color 0.3s ease, color 0.3s ease;
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
        text-align: center;
        text-decoration: none;
        border-radius: 15px;
        padding: 30px;
        box-shadow: 0px 6px 12px rgba(0, 0, 0, 0.2);
        transition: transform 0.3s ease, box-shadow 0.3s ease, background-color 0.3s ease;
        height: 250px;
        position: relative;
      }
  
      .quadrant:hover {
        transform: translateY(-8px);
        box-shadow: 0px 12px 20px rgba(0, 0, 0, 0.3);
      }
  
      .title {
        position: absolute;
        top: 20px;
        font-size: 20px;
        font-weight: 600;
        width: 100%;
        text-align: center;
      }
  
      .toggle-switch {
        position: absolute;
        top: 20px;
        right: 20px;
      }
  
      .switch {
        position: relative;
        display: inline-block;
        width: 40px;
        height: 20px;
      }
  
      .switch input {
        opacity: 0;
        width: 0;
        height: 0;
      }
  
      .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        transition: 0.4s;
        border-radius: 34px;
        box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.2);
      }
  
      .slider:before {
        position: absolute;
        content: "";
        height: 16px;
        width: 16px;
        left: 2px;
        bottom: 2px;
        background-color: white;
        transition: 0.4s;
        border-radius: 50%;
        box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.2);
      }
  
      input:checked + .slider {
        background-color: #66bb6a;
      }
  
      input:checked + .slider:before {
        transform: translateX(20px);
      }
  
      /* Dark Mode */
      body.dark-mode {
        background-color: #1e1e2e;
        color: #e0e0e0;
      }
  
      .dark-mode .quadrant {
        color: #ffffff;
        border: 1px solid #444444;
      }
  
      /* Light Mode */
      body.light-mode {
        background-color: #f2f2f5;
       
      }

      /* Light Mode Title and Description Text Colors */
      .light-mode .quadrant .title {
        color: #1a1a1a; /* Slightly subdued black for titles */
      }
      
      .light-mode .quadrant p {
        color: #4d4d4d; /* More muted black for descriptions */
      }
      
      /* Dark Mode Title and Description Text Colors */
      .dark-mode .quadrant .title p {
        color: #f2f2f2; /* Slightly off-white for titles */
      }
      
      
  
      .light-mode #q1 { background-color: #d7ebff; } /* Light Blue */
      .light-mode #q2 { background-color: #ffdad7; } /* Light Coral */
      .light-mode #q3 { background-color: #ede4ff; } /* Light Purple */
      .light-mode #q4 { background-color: #e6f5e6; } /* Light Green */
      .light-mode #q5 { background-color: #fff5d7; } /* Light Peach */
      .light-mode #q6 { background-color: #d7f5f0; } /* Light Cyan */
      .light-mode #q7 { background-color: #FFEAD7; } /* Light orange */
  
      /* Dark Mode Colors */
      .dark-mode #q1 { background-color: #007bff; } /* Deep Steel Blue */
      .dark-mode #q2 { background-color: #D14141; } /* Muted Deep Red */
      .dark-mode #q3 { background-color: #6A4CA6; } /* Muted Royal Purple */
      .dark-mode #q4 { background-color: #4CAF50; } /* Muted Forest Green */
      .dark-mode #q5 { background-color: #F4C542; } /* Muted yellow */
      .dark-mode #q6 { background-color: #26A69A; } /* Muted Teal Green */
      .dark-mode #q7 { background-color: #FFB74D; } /* Muted Mustard Brown */
  
      
      .light-mode #q3 img {
        filter: brightness(0.0);
      }
      .dark-mode #q3 img {
        filter: brightness(1.0);
      }


      #q3 img {
        width: 105%; 
  
        
      }
    
  
      .quadrant img {
        width: 70%;
        height: 60%;
        object-fit: contain;
        margin-top: 20px;
      }
      
      .quadrant p {
        font-size: 14px;
        margin-top: 15px;
      }
  
      h1 {
        text-align: center;
        font-size: 36px;
        margin-bottom: 40px;
        padding-top: 20px;
      }
    </style>
  </head>
  <body class="light-mode">
    <div class="toggle-switch">
      <label class="switch">
        <input type="checkbox" id="mode-toggle" onclick="toggleMode()">
        <span class="slider"></span>
      </label>
    </div>

    
    <h1>Dynamic Portfolio</h1>
  
    <div class="container">
      <a href="http://localhost:5002" id="q1" class="quadrant">
        <div class="title">Exercise tracker API</div>
        <img src="www.png" alt="Exercise Tracker Image">
        <p>A full stack REST API website for tracking workouts</p>
      </a>
      <a href="http://localhost:3000/run-final" id="q2" class="quadrant">
        <div class="title">OpenGL graphics</div>
        <img src="opengl.png" alt="OpenGL Animation Image">
        <p>Billiards table animation rendered with OpenGL</p>
      </a>
      <a href="/rock-paper-scissors" id="q3" class="quadrant">
        <div class="title">Rock Paper Scissors <br> Lizard Spock</div>
        <img src="RPS.png" alt="Rock Paper Scissors Image">
        <p>A twist on Rock Paper Scissors played against a robot opponent</p>
      </a>
      <a href="https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496" id="q4" class="quadrant">
        <div class="title">Figma prototype</div>
        <img src="figma.png" alt="Figma Example Image">
        <p>Figma prototype for a book review website</p>
      </a>
      <a href="http://localhost:3000/run-weather-app" id="q5" class="quadrant">
        <div class="title">Weather app</div>
        <img src="weather.png" alt="Weather Dashboard Image">
        <p>Displays weather with locational city backgrounds</p>
      </a>
      <a href="/play-video" id="q6" class="quadrant">
        <div class="title">Mobile app</div>
        <img src="app.png" alt="Video Project Image">
        <p>Kotlin Android mobile app video demonstration</p>
      </a>
      <a href="/game" id="q7" class="quadrant">
      <div class="title">Game</div>
      <img src="game.png" alt="Game Image">
      <p>First person maze game</p>
    </a>
    </div>
  
    <script>
      function toggleMode() {
        document.body.classList.toggle('dark-mode');
        document.body.classList.toggle('light-mode');
      }
    </script>
  </body>
  </html>
  
  
  `);
});

app.listen(port, () => {
  console.log(`\nPortfolio homepage running at http://localhost:${port}\n`);
});
