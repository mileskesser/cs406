package com.example.a3.ui

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material.icons.filled.Home
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import com.example.a3.model.CityRecommendation

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun RecommendationListScreen(navController: NavController, category: String, recommendations: List<CityRecommendation>) {
    Column(modifier = Modifier.padding(16.dp)) {
        TopAppBar(
            title = { Text(category) },
            navigationIcon = {
                IconButton(onClick = { navController.navigateUp() }) {
                    Icon(Icons.Default.ArrowBack, contentDescription = "Back")
                }
            },
            actions = {
                IconButton(onClick = { navController.navigate("categories") }) {
                    Icon(Icons.Default.Home, contentDescription = "Home")
                }
            }
        )
        recommendations.forEach { recommendation ->
            Text(
                text = recommendation.name,
                modifier = Modifier
                    .padding(8.dp)
                    .clickable { navController.navigate("recommendation_detail/${recommendation.name}") }
            )
        }
    }
}
