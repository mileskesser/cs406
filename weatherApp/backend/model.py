import json
import pandas as pd
from sklearn.linear_model import LinearRegression

data = {
    'day': [1, 2, 3, 4, 5, 6, 7],
    'temperature': [30, 32, 33, 35, 34, 36, 37]
}
df = pd.DataFrame(data)

X = df[['day']]
y = df['temperature']

model = LinearRegression()
model.fit(X, y)

next_day = [[8]]
prediction = model.predict(next_day)[0]

print(json.dumps({'next_day_temperature': prediction}))