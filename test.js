const { spawn } = require('child_process');
const express = require('express');
const path = require('path');

const app = express();
const port = 3000;

// Array containing project information: name, file path, and port number
const projects = [
  { name: 'Test 1', path: './file1.js', port: 3001 },
  { name: 'Test 2', path: './file2.js', port: 3002 },
  { name: 'Test 3', path: './file3.js', port: 3003 },
  { name: 'Test 4', path: './file4.js', port: 3004 },
  { name: 'OpenGL Animation', type: 'cpp', makePath: '../OpenGL', executable: './sample', port: 4000 }, // Adjusted path
  { name: 'Exercise Tracker App', path: '/Users/mileskesser/Desktop/CS406-main/exercise-app/backend/server.js', port: 5002 },
  { name: 'Rock Paper Scissors Game', url: '/rock-paper-scissors' },
  { name: 'Figma Example', url: 'https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496' }
];

// Serve static files like images from the CS406-main root directory
app.use(express.static(path.join(__dirname, '../')));

// Serve the Rock-Paper-Scissors HTML file directly from the root directory
app.get('/rock-paper-scissors', (req, res) => {
  res.sendFile(path.join(__dirname, '../rock_paper_scissors.html'));  // Corrected path
});

// Route to build and run the OpenGL project
app.get('/run-final', (req, res) => {
  const project = projects.find(p => p.name === 'OpenGL Animation');

  if (project) {
    console.log(`Building and running ${project.name}...`);

    // Step 1: Run `make` to build the project
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

        // Step 2: Run the executable after a successful build
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

        // Send response to the user that the C++ project is running
        res.send(`<h1>${project.name} project is running and will launch momentarily! You can click and drag to interact with the animation</h1>`);
      } else {
        res.send(`<h1>${project.name} failed to build with code ${code}</h1>`);
      }
    });
  } else {
    res.send('<h1>Project not found!</h1>');
  }
});

// Start each Node.js project in a separate process
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

// Express app to serve a homepage linking to all projects
app.get('/', (req, res) => {
  res.send(`
    <html>
      <head>
        <style>
          body, html {
            height: 100%;
            margin: 0;
            font-family: 'Helvetica Neue', Arial, sans-serif;
            background-color: #f4f4f9;
          }
          .container {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            grid-gap: 20px;
            padding: 50px;
            height: 100vh;
            box-sizing: border-box;
          }
          .quadrant {
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 20px;
            color: white;
            text-decoration: none;
            border-radius: 8px;
            padding: 20px;
            transition: transform 0.3s ease, box-shadow 0.3s ease;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
          }
          .quadrant:hover {
            transform: translateY(-5px);
            box-shadow: 0 6px 12px rgba(0, 0, 0, 0.15);
          }
          #q1 { background-color: #1e88e5; }
          #q2 { background-color: #43a047; }
          #q3 { background-color: #fb8c00; }
          #q4 { background-color: #8e24aa; }
          #q5 { background-color: #ff4081; }
          #q6 { background-color: #dc143c; }
          #q7 { background-color: #00796b; }
          #q8 { background-color: #6a1b9a; }
          h1 {
            text-align: center;
            font-size: 36px;
            color: #333;
            margin-bottom: 20px;
          }
        </style>
        <title>Professional Project Portfolio</title>
      </head>
      <body>
        <h1>My Projects</h1>
        <div class="container">
          <a href="http://localhost:3001" id="q1" class="quadrant">Test 1</a>
          <a href="http://localhost:3002" id="q2" class="quadrant">Test 2</a>
          <a href="http://localhost:3003" id="q3" class="quadrant">Test 3</a>
          <a href="http://localhost:3004" id="q4" class="quadrant">Test 4</a>
          <a href="http://localhost:5002" id="q5" class="quadrant">Exercise Tracker App</a>
          <a href="http://localhost:3000/run-final" id="q6" class="quadrant">OpenGL Graphics</a>
          <a href="/rock-paper-scissors" id="q7" class="quadrant">Rock Paper Scissors Game</a>
          <a href="https://www.figma.com/proto/SgjkZcaZmcUWda479hmU1O/Design-Gallery-(Post-your-Clickable-Prototype)?type=design&node-id=27-496&scaling=scale-down&page-id=0%3A1&starting-point-node-id=27%3A496" id="q8" class="quadrant">Figma Example</a>
        </div>
      </body>
    </html>
  `);
});

// Start the Express server
app.listen(port, () => {
  console.log(`\nPortfolio homepage running at http://localhost:${port}\n`);
});
