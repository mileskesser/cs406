const express = require('express');
const axios = require('axios');
const { exec } = require('child_process');
const cors = require('cors');
const app = express();

const WEATHER_API_KEY = 'fdc3e2bb687ce2ad92a928fdff48dc2b';
const UNSPLASH_ACCESS_KEY = 'EYJcNJI1R9CylWP73O737gm7OHDl0dN2fURzD3tbsrg'; // Replace with your Unsplash Access Key
const PORT = 5008;

app.use(cors());

app.get('/weather', async (req, res) => {
  const city = req.query.city || 'New York'; // Default to New York if no city is provided

  try {
    // Fetch weather data from OpenWeather API
    const weatherResponse = await axios.get(`https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${WEATHER_API_KEY}`);
    const weatherData = weatherResponse.data;

    // Fetch city background image from Unsplash API
    const unsplashResponse = await axios.get(`https://api.unsplash.com/search/photos`, {
      params: {
        query: city,
        client_id: UNSPLASH_ACCESS_KEY,
        per_page: 1
      }
    });

    const imageUrl = unsplashResponse.data.results[0]?.urls?.regular || '';

    exec('python3 /Users/mileskesser/Desktop/dynamic-portfolio/weatherApp/backend/model.py', (error, stdout, stderr) => {
      if (error) {
        console.error(`Prediction error: ${error.message}`);
        return res.status(500).send('Error running prediction');
      }
      const prediction = JSON.parse(stdout);

      // Convert temperatures from Kelvin to Fahrenheit
      const tempFahrenheit = ((weatherData.main.temp - 273.15) * 9/5 + 32).toFixed(1);
      const feelsLikeFahrenheit = ((weatherData.main.feels_like - 273.15) * 9/5 + 32).toFixed(1);

      // Render HTML response with weather data, background image, and a search form
      res.send(`
        <html>
          <head>
            <title>Weather Dashboard</title>
            <style>
              * {
                box-sizing: border-box;
                margin: 0;
                padding: 0;
              }
              body {
                font-family: Arial, sans-serif;
                background-image: url('${imageUrl}');
                background-size: cover;
                background-position: center;
                color: #fff;
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: flex-start;
                min-height: 100vh;
                margin: 0;
                padding: 40px 20px;
                backdrop-filter: brightness(0.5);
              }
              .container {
                width: 100%;
                max-width: 600px;
                text-align: center;
                background-color: rgba(0, 0, 0, 0.6);
                border-radius: 8px;
                padding: 20px;
              }
              h1 {
                font-size: 2.5rem;
                color: #00ccff;
                margin-bottom: 20px;
              }
              .search-form {
                display: flex;
                justify-content: center;
                margin-bottom: 30px;
              }
              .search-input {
                padding: 12px;
                font-size: 1rem;
                width: 70%;
                border: 1px solid #ccc;
                border-radius: 5px 0 0 5px;
                outline: none;
              }
              .search-button {
                padding: 12px;
                font-size: 1rem;
                border: none;
                background-color: #00ccff;
                color: white;
                cursor: pointer;
                border-radius: 0 5px 5px 0;
                transition: background-color 0.3s ease;
              }
              .search-button:hover {
                background-color: #0099cc;
              }
              .weather-card {
                background-color: rgba(255, 255, 255, 0.2);
                border-radius: 8px;
                box-shadow: 0 4px 12px rgba(0, 0, 0, 0.5);
                padding: 20px;
                margin-top: 20px;
                text-align: left;
                color: #f0f0f0;
              }
              .weather-card h2 {
                font-size: 1.8rem;
                margin-bottom: 10px;
              }
              .weather-info p {
                font-size: 1.1rem;
                margin: 10px 0;
              }
              .weather-info p strong {
                font-weight: bold;
                color: #ddd;
              }
            </style>
          </head>
          <body>
            <div class="container">
              <h1>Weather Dashboard</h1>

              <!-- Search Form -->
              <form class="search-form" method="get" action="/weather">
                <input
                  type="text"
                  name="city"
                  placeholder="Enter city name"
                  class="search-input"
                  required
                />
                <button type="submit" class="search-button">Search</button>
              </form>

              <!-- Weather Information Display -->
              <div class="weather-card">
                <h2>Weather for ${weatherData.name}</h2>
                <div class="weather-info">
                  <p><strong>Condition:</strong> ${weatherData.weather[0].description}</p>
                  <p><strong>Temperature:</strong> ${tempFahrenheit} °F</p>
                  <p><strong>Feels Like:</strong> ${feelsLikeFahrenheit} °F</p>
                  <p><strong>Humidity:</strong> ${weatherData.main.humidity}%</p>
                  <p><strong>Wind Speed:</strong> ${weatherData.wind.speed} m/s</p>
                </div>
              </div>
            </div>
          </body>
        </html>
      `);
    });
  } catch (error) {
    console.error('Weather API error:', error.response?.data || error.message);
    res.status(500).send(`<h1>Error fetching weather data</h1><p>${error.response?.data?.message || error.message}</p>`);
  }
});

app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}/weather`);
});
