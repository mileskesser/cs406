package com.example.a3.ui

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import com.example.a3.model.CityRecommendation

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun CategoryScreen(navController: NavController, categories: Map<String, List<CityRecommendation>>) {
    Column(modifier = Modifier.padding(16.dp)) {
        TopAppBar(
            title = { Text("Portland, OR") }
        )
        categories.keys.forEach { category ->
            Text(
                text = category,
                modifier = Modifier
                    .padding(8.dp)
                    .clickable { navController.navigate("recommendation_list/$category") }
            )
        }
    }
}
