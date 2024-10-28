// file2.js
const express = require('express');
const app = express();
const port = 3002;

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <h1>Website 2: Change Background Color</h1>
        <button onclick="changeBackgroundColor()">Change Background</button>
        <script>
          function getRandomColor() {
            // Generate a random color in hex format
            const letters = '0123456789ABCDEF';
            let color = '#';
            for (let i = 0; i < 6; i++) {
              color += letters[Math.floor(Math.random() * 16)];
            }
            return color;
          }

          function changeBackgroundColor() {
            // Change the background to a random color
            document.body.style.backgroundColor = getRandomColor();
          }
        </script>
      </body>
    </html>
  `);
});

app.listen(port, () => {
  console.log(`Website 2 running at http://localhost:${port}`);
});
