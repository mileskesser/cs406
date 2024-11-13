package com.example.a3.data

import com.example.a3.R

import com.example.a3.model.CityRecommendation

//Miles Kesser
//OSU
//CS 492

object RecommendationDataSource {
    val categories = mapOf(

        "Coffee Shops" to listOf(
            CityRecommendation("The Albina Press", "4637 N Albina Ave, Portland, OR, 97217", "Artsy cafe dishes up French press coffee & tea along with a variety of pastries in a comfy room", R.drawable.coffee1),
            CityRecommendation("Fresh Pot", "4001 N Mississippi Ave, Portland, OR 97227", "Funky gathering place serving up espresso drinks & baked goods in a chill space.", R.drawable.coffee2),
            CityRecommendation("Tov Coffee", "3639 SE Hawthorne Blvd #5044, Portland, OR 97214", "Quirky cafe in a double-decker bus specializing in Egyptian- & Turkish-style coffee & tea drinks.", R.drawable.coffee3),
            CityRecommendation("Coava Coffee Roasters", "1300 SE GRAND AVE (A), PORTLAND, OR 97214", "Coava Coffee Roasters is a spacious, industrial-chic coffee shop known for its expertly crafted brews and minimalist design.", R.drawable.coffee4)
        ),

        "Restaurants" to listOf(
            CityRecommendation("Cubo de Cuba", "3106 SE Hawthorne Blvd, Portland, OR 97214", "Cuban joint serves up homestyle fare plus cocktails in casual, vibrant surrounds with a patio.", R.drawable.restaurant1),
            CityRecommendation("Cha Cha Cha!", "2635 NE Broadway ste 1, Portland, OR 97232", "Mini-chain taqueria & margarita bar offering locally sourced Mexican fare & some outdoor seating.", R.drawable.restaurant2),
            CityRecommendation("Baby Doll Pizza", "2835 SE Stark St, Portland, OR 97214", "Casual joint with outdoor seats offering locally sourced NY-style pies, draft beer & regional wine.", R.drawable.restaurant3),
            CityRecommendation("Zachs Shack", "4611 SE Hawthorne Blvd, Portland, OR 97215", "Laid-back restaurant/bar with spacious patio & Ping-Pong, serving beer & loaded hot dogs until late.", R.drawable.restaurant4)
        ),

        "Parks" to listOf(
            CityRecommendation("Peninsula Park", "700 N Rosa Parks Way, Portland, OR 97217", "Has public restroom · Dogs allowed · Picnic tables available · large rose garden and fountain  ", R.drawable.park1),
            CityRecommendation("Forest Park", "Portland, OR 97231", "Stretching for more than 8 miles on hillsides overlooking the Willamette River, it is one of the country's largest urban forest", R.drawable.park2),
            CityRecommendation("Laurelhurst Park", "3600 SE Ankeny St, Portland, OR 97214", "Has public restrooms · Dogs Allowed · Picnic tables available · live music and comedy during Summer months", R.drawable.park3),
            CityRecommendation("Mt. Tabor Park", "SE 60th Ave &, SE Salmon St, Portland, OR 97215", "Large forested park covering an extinct volcano with tennis courts, a playground & 360-degree views.", R.drawable.park4)

        )
    )
}