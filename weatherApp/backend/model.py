import json
import pandas as pd
from sklearn.linear_model import LinearRegression

# Simulate some historical weather data
data = {
    'day': [1, 2, 3, 4, 5, 6, 7],
    'temperature': [30, 32, 33, 35, 34, 36, 37]
}
df = pd.DataFrame(data)

X = df[['day']]
y = df['temperature']

# Train a simple model
model = LinearRegression()
model.fit(X, y)

# Predict the temperature for the next day
next_day = [[8]]
prediction = model.predict(next_day)[0]

# Output prediction as JSON
print(json.dumps({'next_day_temperature': prediction}))