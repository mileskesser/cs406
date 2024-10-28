package com.example.a3

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.material3.*
import androidx.compose.material3.Surface
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.example.a3.data.RecommendationDataSource
import com.example.a3.ui.CategoryScreen
import com.example.a3.ui.RecommendationDetailScreen
import com.example.a3.ui.RecommendationListScreen

// Miles Kesser
// OSU
// CS 492

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            val navController: NavHostController = rememberNavController()
            NavHost(
                navController = navController,
                startDestination = "categories"
            ) {
                composable("categories") {
                    CategoryScreen(
                        navController = navController,
                        categories = RecommendationDataSource.categories
                    )
                }
                composable("recommendation_list/{category}") { backStackEntry ->
                    backStackEntry.arguments?.getString("category")?.let { category ->
                        RecommendationListScreen(
                            navController = navController,
                            category = category,
                            recommendations = RecommendationDataSource.categories[category]!!
                        )
                    }
                }
                composable("recommendation_detail/{recommendationName}") { backStackEntry ->
                    backStackEntry.arguments?.getString("recommendationName")?.let { recommendationName ->
                        val recommendation = RecommendationDataSource.categories
                            .values
                            .flatten()
                            .first { it.name == recommendationName }
                        RecommendationDetailScreen(
                            navController = navController,
                            recommendation = recommendation
                        )
                    }
                }
            }
        }
    }
}
