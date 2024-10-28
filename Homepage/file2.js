// website1.js
const express = require('express');
const app = express();
const port = 3002;

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <h1> click me </h1>
        <button onclick="alert('It works!')">Click me</button>
      </body>
    </html>
  `);
});

app.listen(port, () => {
  console.log(`Website 1 running at http://localhost:${port}`);
});
