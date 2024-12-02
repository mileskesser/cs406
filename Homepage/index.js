const { spawn, execSync} = require('child_process');
const express = require('express');
const path = require('path');
const { exec } = require('child_process');

const app = express();
const port = 3000;


const projects = [
  { name: 'OpenGL Animation', type: 'cpp', makePath: '../OpenGL', executable: './sample', port: 4000 },
  { name: 'Exercise Tracker App', path: '/Users/mileskesser/Desktop/dynamic-portfolio/exercise-app/backend/server.js', port: 5002 },
  { name: 'Rock Paper Scissors Game', url: '/rock-paper-scissors' },
  { name: 'Figma Example', url: 'https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496' },
  { name: 'Weather App', path: '/Users/mileskesser/Desktop/dynamic-portfolio/weatherApp/backend/server.js', port: 5008 },
  { name: 'Game', url: '/game' },
  { name: 'Alien Game', url: '/alien' },
  { name: 'Database Visualizer', path: '/Users/mileskesser/Desktop/dynamic-portfolio/database-copy/app.py', port: 5009 },
  { name: 'Geocaching App', path: '/Users/mileskesser/Desktop/dynamic-portfolio/geocaching_app/server.js', port: 3101 }, // Add the Geocaching App here


];

app.use(express.static(path.join(__dirname, '../')));

// Ensure dependencies
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

// Ensure required dependencies are installed
ensureDependencies('sqlite3');

// Route for database visualizer


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
  res.sendFile(path.join(__dirname, '../rock_paper_scissors.html'));  
});

app.get('/game', (req, res) => {
  res.sendFile(path.join(__dirname, '../game.html'));
});


app.get('/alien', (req, res) => {
  res.sendFile(path.join(__dirname, '../alien.html'));
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
                h1, p { /* Set the same color for both */
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
      } else {
        res.send(`
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
                h1, p { /* Set the same color for both */
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
    });
  } else {
    res.send(`
      <html>
        <head>
          <title>Project Not Found</title>
          <style>
            body {
              font-family: Arial, sans-serif;
              background-color: #121212;
              color: #f5f5f5;
              text-align: center;
              padding: 50px;
              margin: 0;
            }
            h1, p { /* Set the same color for both */
              font-size: 2rem;
              color: #ffffff;
              margin-bottom: 20px;
            }
          </style>
        </head>
        <body>
          <h1>Project Not Found!</h1>
        </body>
      </html>
    `);
  }
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

// Route to launch the Geocaching App
app.get('/run-geocaching', (req, res) => {
  const project = projects.find(p => p.name === 'Geocaching App');

  if (project && project.path) {
    console.log(`Starting ${project.name} on port ${project.port}...`);

    // Run the Geocaching App server
    const geocachingProcess = spawn('node', [project.path]);

    geocachingProcess.stdout.on('data', (data) => {
      console.log(`${project.name} output: ${data}`);
    });

    geocachingProcess.stderr.on('data', (data) => {
      console.error(`${project.name} error: ${data}`);
    });

    geocachingProcess.on('close', (code) => {
      console.log(`${project.name} process exited with code ${code}`);
    });

    // Redirect to the appropriate port
    res.redirect(`http://localhost:${project.port}`);
  } else {
    res.status(404).send('<h1>Geocaching App project not found!</h1>');
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
        grid-template-columns: repeat(5, 250px); /* 5 fixed-width columns */
        grid-gap: 20px; /* Space between cards */
        padding: 60px;
        justify-content: center; /* Center horizontally */
        align-content: center; /* Center vertically if the grid height grows */
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
      
      
      
  
      .light-mode #q1 { background-color: #d7ebff; } /* Light Blue */
      .light-mode #q2 { background-color: #ffdad7; } /* Light Coral */
      .light-mode #q3 { background-color: #ede4ff; } /* Light Purple */
      .light-mode #q4 { background-color: #e6f5e6; } /* Light Green */
      .light-mode #q5 { background-color: #fff5d7; } /* Light yellow */
      .light-mode #q6 { background-color: #e6f5e6; } 
      .light-mode #q7 { background-color: #fff5d7; } 
      .light-mode #q8 { background-color: #ede4ff; } 
      .light-mode #q9 { background-color: #d7ebff; } 
      .light-mode #q10 { background-color: #ffdad7; } 


  
      
      .light-mode #q3 img {
        filter: brightness(0.0);
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


   


     
      <a href="/rock-paper-scissors" id="q3" class="quadrant">
        <div class="title">Rock Paper Scissors <br> Lizard Spock</div>
        <img src="RPS.png" alt="Rock Paper Scissors Image">
        <p>A twist on Rock Paper Scissors played against a robot opponent</p>
      </a>



      <a href="/game" id="q7" class="quadrant">
      <div class="title">Maze Game</div>
      <img src="game.png" alt="Game Image">
      <p>First person maze game</p>
    </a>


      <a href="/alien" id="q8" class="quadrant">
      <div class="title">Alien Game</div>
      <img src="alien.png" alt="Alien Image">
      <p>Arcade style laser dodging game</p>
    </a>
 
      

      <a href="/run-database" id="q9" class="quadrant">
      <div class="title">Database Visualizer</div>
      <img src="database.png" alt="Database Image">
      <p>Interactive SQL database viewer</p>
    </a>

    <a  href="/run-geocaching" id="q10" class="quadrant">
    <div class="title">Geocache</div>
    <img src="geocache.png" alt="Geocache Image">
    <p>A geocaching scavenger hunt</p>
  </a>

    


  <a href="/play-video" id="q6" class="quadrant">
        <div class="title">Mobile app</div>
        <img src="app.png" alt="Video Project Image">
        <p>Kotlin Android mobile app video demonstration</p>
      </a>

   
    


 


    </div>
  
    
  </body>
  </html>
  
  
  `);
});

app.listen(port, () => {
  console.log(`\nPortfolio homepage running at http://localhost:${port}\n`);
});
