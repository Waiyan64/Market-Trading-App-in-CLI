#include "Wallet.h"
#include <iostream>
#include <map>
#include <algorithm>
#include "MerkelMain.h"
#include "Candlestick.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"

std::vector<Candlestick> computeCandlestickData(std::vector<OrderBookEntry>& orders, std::string product)
{
    std::vector<Candlestick> candlesticks;
        // Sort the orders by timestamp
        std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);

        // Initialize variables to keep track of the current and previous timestamps
        std::string currentTimestamp = orders[0].timestamp;
        std::string previousTimestamp = "";

        // Initialize variables to keep track of the total price and amount for the current and previous timestamps
        double currentTotalPrice = 0;
        double currentTotalAmount = 0;
        double previousTotalPrice = 0;
        double previousTotalAmount = 0;

        // Initialize variables to keep track of the highest and lowest prices for the current timestamp
        double currentHigh = orders[0].price;
        double currentLow = orders[0].price;

        for (OrderBookEntry& order : orders) {
            if (order.product == product) {
                if (order.timestamp == currentTimestamp) {
                    // Update the total price and amount for the current timestamp
                    currentTotalPrice += order.price * order.amount;
                    currentTotalAmount += order.amount;

                    // Update the highest and lowest prices for the current timestamp
                    currentHigh = std::max(currentHigh, order.price);
                    currentLow = std::min(currentLow, order.price);
                }
                else {
                    // Compute the open and close prices for the current timestamp
                    double open = previousTotalAmount == 0 ? 0 : previousTotalPrice / previousTotalAmount;
                    double close = currentTotalAmount == 0 ? 0 : currentTotalPrice / currentTotalAmount;

                    // Add a new Candlestick object to the vector
                    candlesticks.push_back(Candlestick(currentTimestamp, open, currentHigh, currentLow, close));

                    // Update the current and previous timestamps
                    previousTimestamp = currentTimestamp;
                    currentTimestamp = order.timestamp;

                    // Update the total price and amount for the current and previous timestamps
                    previousTotalPrice = currentTotalPrice;
                    previousTotalAmount = currentTotalAmount;
                    currentTotalPrice = order.price * order.amount;
                    currentTotalAmount = order.amount;

                    // Update the highest and lowest prices for the current timestamp
                    currentHigh = order.price;
                    currentLow = order.price;
                }
            }
        }

        // Compute the open and close prices for the last timestamp
        double open = previousTotalAmount == 0 ? 0 : previousTotalPrice / previousTotalAmount;
        double close = currentTotalAmount == 0 ? 0 : currentTotalPrice / currentTotalAmount;

        // Add a new Candlestick object to the vector
        candlesticks.push_back(Candlestick(currentTimestamp, open, currentHigh, currentLow, close));

        return candlesticks;
    }


void plotCandlesticks(const std::vector<Candlestick>& candlesticks) {
    // Define the height of the plot
    const int plotHeight = 20;

    // Check if the vector is empty
    if (candlesticks.empty()) {
        std::cerr << "Error: No candlestick data available." << std::endl;
        return;
    }

    // Find the minimum and maximum price
    double minPrice = std::numeric_limits<double>::max();
    double maxPrice = std::numeric_limits<double>::min();
    for (const Candlestick& candlestick : candlesticks) {
        minPrice = std::min(minPrice, candlestick.low);
        maxPrice = std::max(maxPrice, candlestick.high);
    }

    // Compute the price range
    double priceRange = maxPrice - minPrice;

    // Iterate over the candlesticks
    for (const Candlestick& candlestick : candlesticks) {
        // Normalize the open, high, low, and close prices to the plot height
        int open = plotHeight - static_cast<int>((candlestick.open - minPrice) / priceRange * plotHeight);
        int high = plotHeight - static_cast<int>((candlestick.high - minPrice) / priceRange * plotHeight);
        int low = plotHeight - static_cast<int>((candlestick.low - minPrice) / priceRange * plotHeight);
        int close = plotHeight - static_cast<int>((candlestick.close - minPrice) / priceRange * plotHeight);

        // Create a vector to represent the plot
        std::vector<char> plot(plotHeight, ' ');

        // Draw the high-low line
        for (int i = high; i <= low; ++i) {
            if (i >= 0 && i < plotHeight) {
                plot[i] = '|';
            }
        }

        // Draw the open-close box
        if (open >= 0 && open < plotHeight) {
            plot[open] = '-';
        }
        if (close >= 0 && close < plotHeight) {
            plot[close] = '-';
        }

        // Print the plot
        for (char c : plot) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void drawLineGraph(std::vector<int> data, int height) {
    // Create a 2D array to represent the graph
    std::vector<std::vector<char>> graph(height, std::vector<char>(data.size(), ' '));

    // Plot the points on the graph
    for (int i = 0; i < data.size(); i++) {
        if (data[i] >= 0 && data[i] < height) {
            graph[height - data[i] - 1][i] = '.';
        }
    }

    // Print the graph
    for (const auto& row : graph) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << '\n';
    }
}

int main()
{
   // MerkelMain app{};
   // app.init();
    CSVReader csvReader;
    std::vector<OrderBookEntry> orders = csvReader.readCSV("20200317.csv");
    
    // Call the computeCandlestickData function
    std::vector<Candlestick> candlesticks = computeCandlestickData(orders, "ETH/BTC");

    // Print the candlestick data
    for (Candlestick& candlestick : candlesticks) {
        candlestick.printData();
    }
    plotCandlesticks(candlesticks);

    std::vector<int> data = { 1, 2, 3, 4, 5, 4, 3, 2, 1 };

    // Height of the graph
    int height = 6;

    // Draw the line graph
    drawLineGraph(data, height);



    return 0;
   

}
