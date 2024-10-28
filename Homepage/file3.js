// website3.js
const express = require('express');
const app = express();
const port = 3003;

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <h1>Website 3: Increment Counter</h1>
        <p>Counter: <span id="counter">0</span></p>
        <button onclick="incrementCounter()">Increment Counter</button>
        <script>
          let count = 0;
          function incrementCounter() {
            count++;
            document.getElementById('counter').innerText = count;
          }
        </script>
      </body>
    </html>
  `);
});

app.listen(port, () => {
  console.log(`Website 3 running at http://localhost:${port}`);
});
