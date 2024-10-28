// website1.js
const express = require('express');
const app = express();
const port = 3001;

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <h1>Website 1: Alert Button</h1>
        <button onclick="alert('Button Clicked!')">Click me</button>
      </body>
    </html>
  `);
});

app.listen(port, () => {
  console.log(`Website 1 running at http://localhost:${port}`);
});
