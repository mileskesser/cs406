document.getElementById('searchButton').addEventListener('click', () => {
    const city = document.getElementById('cityInput').value;
    if (city) {
        fetchWeather(city);
    }
});

async function fetchWeather(city) {
    try {
        const response = await fetch(`http://localhost:5000/weather?city=${city}`);
        const data = await response.json();
        
        if (data.weatherData && data.prediction) {
            displayWeather(data.weatherData, data.prediction);
        } else {
            alert("Weather data not available for the specified location.");
        }
    } catch (error) {
        console.error("Error fetching weather:", error);
        alert("An error occurred while fetching weather data.");
    }
}

function displayWeather(weatherData, prediction) {
    const weatherDiv = document.getElementById('weather');
    weatherDiv.innerHTML = `
        <div class="weather-title">${weatherData.name} Weather</div>
        <div class="weather-info">
            <p><strong>Condition:</strong> ${weatherData.weather[0].description}</p>
            <p><strong>Temperature:</strong> ${(weatherData.main.temp - 273.15).toFixed(1)} °C</p>
            <p><strong>Feels Like:</strong> ${(weatherData.main.feels_like - 273.15).toFixed(1)} °C</p>
            <p><strong>Humidity:</strong> ${weatherData.main.humidity}%</p>
            <p><strong>Wind Speed:</strong> ${weatherData.wind.speed} m/s</p>
            <p><strong>Next Day Temperature Prediction:</strong> ${prediction.next_day_temperature.toFixed(1)} °C</p>
        </div>
    `;
    weatherDiv.style.display = 'block';
}document.getElementById('searchButton').addEventListener('click', () => {
    const city = document.getElementById('cityInput').value;
    if (city) {
        fetchWeather(city);
    }
});

async function fetchWeather(city) {
    try {
        const response = await fetch(`http://localhost:5000/weather?city=${city}`);
        const data = await response.json();
        
        if (data.weatherData && data.prediction) {
            displayWeather(data.weatherData, data.prediction);
        } else {
            alert("Weather data not available for the specified location.");
        }
    } catch (error) {
        console.error("Error fetching weather:", error);
        alert("An error occurred while fetching weather data.");
    }
}

function displayWeather(weatherData, prediction) {
    const weatherDiv = document.getElementById('weather');
    weatherDiv.innerHTML = `
        <div class="weather-title">${weatherData.name} Weather</div>
        <div class="weather-info">
            <p><strong>Condition:</strong> ${weatherData.weather[0].description}</p>
            <p><strong>Temperature:</strong> ${(weatherData.main.temp - 273.15).toFixed(1)} °C</p>
            <p><strong>Feels Like:</strong> ${(weatherData.main.feels_like - 273.15).toFixed(1)} °C</p>
            <p><strong>Humidity:</strong> ${weatherData.main.humidity}%</p>
            <p><strong>Wind Speed:</strong> ${weatherData.wind.speed} m/s</p>
            <p><strong>Next Day Temperature Prediction:</strong> ${prediction.next_day_temperature.toFixed(1)} °C</p>
        </div>
    `;
    weatherDiv.style.display = 'block';
}
