package com.example.a3.ui

import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.Home
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import com.example.a3.model.CityRecommendation

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun RecommendationDetailScreen(navController: NavController, recommendation: CityRecommendation) {
    Column(modifier = Modifier.padding(16.dp)) {
        TopAppBar(
            title = { Text(recommendation.name) },
            navigationIcon = {
                IconButton(onClick = { navController.navigateUp() }) {
                    Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "Back")
                }
            },
            actions = {
                IconButton(onClick = { navController.navigate("categories") }) {
                    Icon(Icons.Default.Home, contentDescription = "Home")
                }
            }
        )
        Image(
            painter = painterResource(id = recommendation.imageRes),
            contentDescription = null,
            modifier = Modifier
                .fillMaxWidth()
                .height(300.dp)
                .background(MaterialTheme.colorScheme.surface)
        )
        Spacer(modifier = Modifier.height(8.dp))
        Text(
            text = recommendation.address,
            modifier = Modifier.padding(8.dp)
        )
        Text(
            text = recommendation.description,
            modifier = Modifier.padding(8.dp)
        )
    }
}
