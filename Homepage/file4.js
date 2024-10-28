// website4.js
const express = require('express');
const app = express();
const port = 3004;

app.get('/', (req, res) => {
  res.send(`
    <html>
      <body>
        <h1>Website 4: Random Joke Generator</h1>
        <p id="joke">Click the button to get a joke!</p>
        <button onclick="getJoke()">Get Joke</button>
        <script>
          async function getJoke() {
            const response = await fetch('https://official-joke-api.appspot.com/random_joke');
            const joke = await response.json();
            document.getElementById('joke').innerText = joke.setup + ' - ' + joke.punchline;
          }
        </script>
      </body>
    </html>
  `);
});

app.listen(port, () => {
  console.log(`Website 4 running at http://localhost:${port}`);
});
