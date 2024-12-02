const express = require('express');
const path = require('path');
const app = express();
const port = 3101;

const geocaches = [
    {
        clue: "Test your device's GPS functionality to ensure accurate location tracking.",
        latitude: 45.51154679582722,
        longitude: -122.6138918571162,
        buttonText: "Test Location Tracking" // Custom button text for the first clue
    },
    {
        clue: "Your next clue can be found at the hardware store with the wooden mascot.",
        latitude: 45.519400153385064,
        longitude: -122.58441910753297,
        buttonText: "Found It!" // Default button text for subsequent clues
    },
    {
        clue: "Finally, find the opposite of the pancake door.",
        latitude: 45.51182674238814,
        longitude: -122.62656828109401,
        buttonText: "Found It!" // Default button text for subsequent clues
    }
];

app.use(express.static(path.join(__dirname, 'public')));

// Function to calculate the distance in kilometers
function getDistance(lat1, lon1, lat2, lon2) {
    const R = 6371; // Radius of Earth in kilometers
    const toRadians = (degrees) => degrees * (Math.PI / 180);
    const dLat = toRadians(lat2 - lat1);
    const dLon = toRadians(lon2 - lon1);
    const a =
        Math.sin(dLat / 2) * Math.sin(dLat / 2) +
        Math.cos(toRadians(lat1)) * Math.cos(toRadians(lat2)) *
        Math.sin(dLon / 2) * Math.sin(dLon / 2);
    const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
    const distanceKm = R * c; // Distance in kilometers
    const distanceMiles = distanceKm * 0.621371; // Convert to miles
    return { distanceKm, distanceMiles }; // Return both distances
}

// Endpoint to get the next geocache clue and location
app.get('/geocache/:index', (req, res) => {
    const index = parseInt(req.params.index, 10);
    if (index >= 0 && index < geocaches.length) {
        const clue = geocaches[index];
        res.json(clue); // Send the full clue object, including buttonText
    } else {
        res.json({ end: true, message: "You've completed the geocaching scavenger hunt!" });
    }
});

// Endpoint to check user's location
// Endpoint to check user's location
app.get('/check-location/:index', (req, res) => {
    const index = parseInt(req.params.index, 10);
    const userLat = parseFloat(req.query.lat);
    const userLon = parseFloat(req.query.lon);

    if (index >= 0 && index < geocaches.length) {
        const target = geocaches[index];
        const { distanceKm, distanceMiles } = getDistance(userLat, userLon, target.latitude, target.longitude);

        if (distanceKm < 0.02) {
            if (index === 0) {
                // Special message for the first clue (location tracking test)
                res.json({ success: true, message: "Location tracking is accurate!" });
            } else {
                // Default success message for subsequent clues
                res.json({ success: true, message: "Congratulations! You found it!" });
            }
        } else {
            res.json({
                success: false,
                message: `Not quite! You're ${distanceMiles.toFixed(2)} miles away from the correct location.`,
                distanceKm: distanceKm.toFixed(2),
                distanceMiles: distanceMiles.toFixed(2)
            });
        }
    } else {
        res.status(404).json({ error: "Invalid geocache index!" });
    }
});

app.listen(port, () => {
    console.log(`Geocaching app running at http://localhost:${port}`);
});
