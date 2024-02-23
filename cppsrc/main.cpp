/* cppsrc/main.cpp */
#include <napi.h>
#include <vector>
#include <string>
#include <map>
// include cout
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;


std::vector<std::string> splitIntoWords(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

double DjaroWinklerDistance(const std::string& a, const std::string& b) {
    const int m = a.length();
    const int n = b.length();
    const int threshold = std::max(m, n) / 2 - 1;
    std::vector<bool> matchesA(m, false);
    std::vector<bool> matchesB(n, false);
    int matches = 0;
    int transpositions = 0;

    for (int i = 0; i < m; i++) {
        const int start = std::max(0, i - threshold);
        const int end = std::min(i + threshold + 1, n);
        for (int j = start; j < end; j++) {
            if (b[j] == a[i] && !matchesB[j]) {
                matchesA[i] = true;
                matchesB[j] = true;
                matches++;
                break;
            }
        }
    }

    if (matches == 0) {
        return 0.0;
    }

    int k = 0;
    for (int i = 0; i < m; i++) {
        if (matchesA[i]) {
            while (!matchesB[k]) {
                k++;
            }
            if (a[i] != b[k]) {
                transpositions++;
            }
            k++;
        }
    }

    const double jaroDistance = (matches / static_cast<double>(m) + matches / static_cast<double>(n) + (matches - transpositions / 2.0) / matches) / 3.0;
    const double p = 0.1;
    const double jaroWinklerDistance = jaroDistance + std::min(0.1, 1.0 / m) * p * (1.0 - jaroDistance);
    return jaroWinklerDistance;
}

// a function that takes a string, an array of strings, and a threshold and
// uses the Jaro-Winkler distance algorithm to return an array of strings
// in order of similarity to the input string
vector<string> jaroWinklerDistance(const std::string& a, const vector<string>& arr, const double threshold) {
    std::vector<std::pair<double, std::string>> distances;
    // cout the input string
    cout << "Input String: " << a << endl;
    for (const auto& b : arr) {
        const double distance = DjaroWinklerDistance(a, b);
        if (distance >= threshold) {
            distances.push_back(std::make_pair(distance, b));
        }
    }
    std::sort(distances.begin(), distances.end(), std::greater<std::pair<double, std::string>>());
    std::vector<string> result;
    for (const auto& distance : distances) {
        result.push_back(distance.second);
    }
    return result;
}

double calculateDistanceBetweenWordSets(const std::vector<std::string>& set1, const std::vector<std::string>& set2) {
    double maxDistance = 0.0;

    // Assume DjaroWinklerDistance is a function you've defined to calculate the Jaro-Winkler distance between two strings.
    for (const auto& word1 : set1) {
        for (const auto& word2 : set2) {
            double distance = DjaroWinklerDistance(word1, word2);
            maxDistance = std::max(maxDistance, distance);
        }
    }

    return maxDistance;
}


// Define a custom type for clarity and ease of use
typedef std::pair<double, std::string> DistanceAndString;
typedef std::pair<int, double> IndexAndDistance;
// The new function
typedef std::pair<int, double> IndexAndDistance;

std::vector<IndexAndDistance> objectJaroWinklerSearch(
    const std::string& targetString,
    const std::vector<std::map<std::string, std::string>>& dataObjects,
    const std::vector<std::string>& keys,
    double threshold) {
    std::vector<IndexAndDistance> results;

    for (size_t i = 0; i < dataObjects.size(); ++i) {
        const auto& dataObject = dataObjects[i];
        for (const auto& key : keys) {
            auto it = dataObject.find(key);
            if (it != dataObject.end()) {
                // Split targetString and dataObject value into words
                auto targetWords = splitIntoWords(targetString);
                auto valueWords = splitIntoWords(it->second);

                // Logic to compare words and calculate a "distance" or matching score
                double distance = calculateDistanceBetweenWordSets(targetWords, valueWords);

                if (distance >= threshold) {
                    results.emplace_back(i, distance);
                    break; // Assuming you only need one match per object
                }
            }
        }
    }

    // Sort results based on the distance, highest first
    std::sort(results.begin(), results.end(), [](const IndexAndDistance& a, const IndexAndDistance& b) {
        return a.second > b.second; // Assuming higher distance means a better match
    });

    return results;
}

// the same as JarowinklerDistanceMethod but for objectJaroWinklerSearch
Napi::Array JaroWinklerDistanceObjectMethod(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::String a = info[0].As<Napi::String>();
    Napi::Array objectsArray = info[1].As<Napi::Array>();
    Napi::Number threshold = info[3].As<Napi::Number>();

    std::vector<std::string> keys;
    // Extract keys from the third parameter, which should be an array of strings.
    Napi::Array napiKeys = info[2].As<Napi::Array>();
    for (uint32_t i = 0; i < napiKeys.Length(); ++i) {
        Napi::Value keyVal = napiKeys.Get(i);
        if (keyVal.IsString()) {
            keys.push_back(keyVal.As<Napi::String>());
        }
    }

    std::vector<std::map<std::string, std::string>> objects;
    // Iterate over each object in the input array
    for (uint32_t i = 0; i < objectsArray.Length(); ++i) {
        Napi::Value objValue = objectsArray.Get(i);
        if (!objValue.IsObject()) {
            continue; // Skip non-object items
        }
        Napi::Object obj = objValue.As<Napi::Object>();
        std::map<std::string, std::string> cppObj;
        for (const auto& key : keys) {
            Napi::Value val = obj.Get(key);
            if (val.IsString()) {
                cppObj[key] = val.As<Napi::String>();
            }
        }
        objects.push_back(cppObj);
    }

    std::string targetStr = a.Utf8Value();
    double searchThreshold = threshold.DoubleValue();
    std::vector<IndexAndDistance> matchingResults = objectJaroWinklerSearch(a.Utf8Value(), objects, keys, threshold.DoubleValue());

    // Create a new JS array to return the matching objects in sorted order
    Napi::Array matchingObjects = Napi::Array::New(env);
    for (size_t i = 0; i < matchingResults.size(); ++i) {
        // Use the index part of each pair to access the original JS object
        matchingObjects.Set(i, objectsArray.Get(matchingResults[i].first));
    }

    return matchingObjects;
}

Napi::Array JaroWinklerDistanceMethod(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    Napi::String a = info[0].As<Napi::String>();
    Napi::Array arr = info[1].As<Napi::Array>();
    Napi::Number threshold = info[2].As<Napi::Number>();
    vector<string> cppArr;
    for (int i = 0; i < arr.Length(); i++) {
        cppArr.push_back(arr.Get(i).As<Napi::String>());
    }
    vector<string> result = jaroWinklerDistance(a, cppArr, threshold);

    Napi::Array returnArr = Napi::Array::New(env, result.size());
    for (int i = 0; i < result.size(); i++) {
        returnArr.Set(i, result[i]);
    }
    return returnArr;
}




Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "array"),
       Napi::Function::New(env, JaroWinklerDistanceMethod)); // Named Export 1

  exports.Set(Napi::String::New(env, "object"),
           Napi::Function::New(env, JaroWinklerDistanceObjectMethod)); // Named Export 2

  return exports;
}

NODE_API_MODULE(testaddon, Init)
