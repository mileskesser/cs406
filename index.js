const { spawn, execSync } = require('child_process');
const express = require('express');
const path = require('path');
const { exec } = require('child_process');
const fs = require('fs');  

const app = express();
const port = process.env.PORT || 3000;


const BASE_DIR = __dirname;
//app.use(express.static(path.join(__dirname)));


const projects = [
  {
    name: 'OpenGL Animation',
    type: 'cpp',
    makePath: path.join(BASE_DIR, 'OpenGL'),
    executable: './sample',
    port: 4000,
  },
  {
    name: 'Exercise Tracker App',
    path: path.join(BASE_DIR, 'exercise-app', 'backend', 'server.js'),
    port: 5002,
  },
  {
    name: 'Geocaching App',
    path: path.join(BASE_DIR, 'geocaching_app', 'server.js'),
    port: 3101,
  },
  {
    name: 'Weather App',
    path: path.join(BASE_DIR, 'weatherApp', 'backend', 'server.js'),
    port: 5008,
  },
  {
    name: 'Database Visualizer',
    path: path.join(BASE_DIR, 'database-copy', 'app.py'),
    port: 5009,
  },
];


app.use(express.static(BASE_DIR));
//app.use(express.static(path.join(__dirname, '../')));


function ensureDependencies(dependency) {
  try {
    require.resolve(dependency);
    console.log(`${dependency} is already installed.`);
  } catch (err) {
    console.log(`${dependency} is not installed. Installing now...`);
    execSync(`npm install ${dependency}`, { stdio: 'inherit' });
    console.log(`${dependency} has been successfully installed.`);
  }
}

ensureDependencies('sqlite3');


app.get('/', (req, res) => {
  res.sendFile(path.join(BASE_DIR, 'index.html'));
});






app.get('/run-final', (req, res) => {
  const project = projects.find((p) => p.name === 'OpenGL Animation');

  if (!project) {
      return res.status(404).send('<h1>Project Not Found!</h1>');
  }

  const makePath = path.join(__dirname, 'OpenGL');
  const executablePath = path.join(makePath, 'sample');

  console.log(`Building and running ${project.name} in ${makePath}...`);

  const makeProcess = spawn('/usr/bin/make', [], { cwd: makePath });

  makeProcess.stdout.on('data', (data) => console.log(`${project.name} build output: ${data}`));
  makeProcess.stderr.on('data', (data) => console.error(`${project.name} build error: ${data}`));

  makeProcess.on('close', (code) => {
      if (code !== 0) {
          return res.send(`
            <html>
              <head>
                <title>${project.name} - Build Failed</title>
                <style>
                  body {
                    font-family: Arial, sans-serif;
                    background-color: #121212;
                    color: #f5f5f5;
                    text-align: center;
                    padding: 50px;
                    margin: 0;
                  }
                  h1, p { 
                    font-size: 2rem;
                    color: #ffffff;
                    margin-bottom: 20px;
                  }
                </style>
              </head>
              <body>
                <h1>${project.name} Build Failed</h1>
                <p>The build process exited with code ${code}.</p>
              </body>
            </html>
          `);
      }

      console.log(`${project.name} built successfully.`);

      if (!fs.existsSync(executablePath)) {
          console.error(`❌ Executable not found at ${executablePath}`);
          return res.send(`
            <html>
              <head>
                <title>Executable Not Found</title>
                <style>
                  body {
                    font-family: Arial, sans-serif;
                    background-color: #121212;
                    color: #f5f5f5;
                    text-align: center;
                    padding: 50px;
                    margin: 0;
                  }
                  h1, p { 
                    font-size: 2rem;
                    color: #ffffff;
                    margin-bottom: 20px;
                  }
                </style>
              </head>
              <body>
                <h1>Error: Executable Not Found</h1>
                <p>The OpenGL executable could not be found after compilation.</p>
              </body>
            </html>
          `);
      }

      console.log(`✅ Found executable at ${executablePath}, launching OpenGL...`);
      fs.chmodSync(executablePath, '755');

      let runCommand;
      if (process.platform === 'darwin') {
          runCommand = `osascript -e 'tell app "Terminal" to do script "cd ${makePath} && ./sample"'`;
      } else if (process.platform === 'win32') {
          runCommand = `start cmd.exe /c "cd /d ${makePath} && sample"`;
      } else {
          runCommand = `x-terminal-emulator -e "cd ${makePath} && ./sample"`;
      }

      exec(runCommand, (error, stdout, stderr) => {
          if (error) {
              console.error(`Execution error: ${error.message}`);
              return res.send(`
                <html>
                  <head>
                    <title>Execution Failed</title>
                    <style>
                      body {
                        font-family: Arial, sans-serif;
                        background-color: #121212;
                        color: #f5f5f5;
                        text-align: center;
                        padding: 50px;
                        margin: 0;
                      }
                      h1, p { 
                        font-size: 2rem;
                        color: #ffffff;
                        margin-bottom: 20px;
                      }
                    </style>
                  </head>
                  <body>
                    <h1>Error Launching OpenGL</h1>
                    <p>The OpenGL animation failed to launch.</p>
                  </body>
                </html>
              `);
          }
          console.log(`🎉 OpenGL animation launched: ${stdout}`);
          res.send(`
            <html>
              <head>
                <title>${project.name} - Running</title>
                <style>
                  body {
                    font-family: Arial, sans-serif;
                    background-color: #121212;
                    color: #f5f5f5;
                    text-align: center;
                    padding: 50px;
                    margin: 0;
                  }
                  h1, p { 
                    font-size: 2rem;
                    color: #ffffff;
                    margin-bottom: 20px;
                  }
                  .instructions {
                    color: #ffcc00;
                    font-style: italic;
                  }
                </style>
              </head>
              <body>
                <h1>${project.name} Project is Running!</h1>
                <p>The animation will launch momentarily.</p>
                <p class="instructions">You can click and drag to interact with the animation.</p>
              </body>
            </html>
          `);
      });
  });
});








app.get('/run-database', (req, res) => {
  const project = projects.find(p => p.name === 'Database Visualizer');
  
  if (project && project.path) {
      console.log(`Starting ${project.name} on port ${project.port}...`);

      // Run the Python script
      const pythonProcess = spawn('python3', [project.path]);

      pythonProcess.stdout.on('data', (data) => {
          console.log(`${project.name} output: ${data}`);
      });

      pythonProcess.stderr.on('data', (data) => {
          console.error(`${project.name} error: ${data}`);
      });

      pythonProcess.on('close', (code) => {
          console.log(`${project.name} process exited with code ${code}`);
      });

      // Redirect to the appropriate port
      res.redirect(`http://localhost:${project.port}`);
  } else {
      res.status(404).send('<h1>Database Visualizer project not found!</h1>');
  }
});





app.get('/rock-paper-scissors', (req, res) => {
  res.sendFile(path.join(BASE_DIR, 'rock_paper_scissors.html'));  
});

// ✅ Run Game
app.get('/game', (req, res) => {
  res.sendFile(path.join(BASE_DIR, 'game.html'));
});


// ✅ Run Alien Game
app.get('/alien', (req, res) => {
  res.sendFile(path.join(BASE_DIR, 'alien.html'));
});



// ✅ Start Weather App
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


// ✅ Start Geocaching App
app.get('/run-geocaching', (req, res) => {
  const project = projects.find(p => p.name === 'Geocaching App');
  if (project) {
    console.log(`Starting ${project.name} on port ${project.port}...`);
    const geocachingProcess = spawn('node', [project.path]);

    geocachingProcess.stdout.on('data', (data) => console.log(`${project.name} output: ${data}`));
    geocachingProcess.stderr.on('data', (data) => console.error(`${project.name} error: ${data}`));

    geocachingProcess.on('close', (code) => console.log(`${project.name} process exited with code ${code}`));
    res.redirect(`http://localhost:${project.port}`);
  } else {
    res.send('<h1>Geocaching App project not found!</h1>');
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


app.listen(port, '0.0.0.0', () => {
  console.log(`\n🌍 Portfolio homepage running at http://localhost:${port} (accessible from any device!)\n`);
});
