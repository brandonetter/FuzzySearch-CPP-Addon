/* cppsrc/main.cpp */
#include <napi.h>
using namespace std;

double jaroWinklerDistance(const std::string& a, const std::string& b) {
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
    for (const auto& b : arr) {
        const double distance = jaroWinklerDistance(a, b);
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

// Define a custom type for clarity and ease of use
typedef std::pair<double, std::string> DistanceAndString;

// The new function
std::vector<std::string> objectJaroWinklerSearch(
    const std::string& targetString,
    const std::map<std::string, std::string> &dataObject,
    const std::vector<std::string>& keys,
    const double threshold)
{
    std::vector<DistanceAndString> distances;

    // Iterate over keys of interest within the object
    for (const auto& key : keys) {
        // Ensure the key exists
        if (dataObject.find(key) != dataObject.end()) {
            const std::string& value = dataObject.at(key);

            // Calculate Jaro-winkler distance
            const double distance = jaroWinklerDistance(targetString, value);

            if (distance >= threshold) {
                distances.push_back(std::make_pair(distance, value));
            }
        }
    }

    // Sort results (descending order based on distance)
    std::sort(distances.begin(), distances.end(), std::greater<DistanceAndString>());

    // Extract just the strings
    std::vector<std::string> result;
    for (const auto& distanceAndValue : distances) {
        result.push_back(distanceAndValue.second);
    }

    return result;
}
// the same as JarowinklerDistanceMethod but for objectJaroWinklerSearch
Napi::Array JaroWinklerDistanceObjectMethod(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    Napi::String a = info[0].As<Napi::String>();
    Napi::Object obj = info[1].As<Napi::Object>();
    Napi::Array keys = info[2].As<Napi::Array>();
    Napi::Number threshold = info[3].As<Napi::Number>();
    std::map<std::string, std::string> cppObj;
    for (int i = 0; i < keys.Length(); i++) {
        std::string key = keys.Get(i).As<Napi::String>();
        cppObj[key] = obj.Get(key).As<Napi::String>();
    }
    vector<string> result = objectJaroWinklerSearch(a, cppObj, keys, threshold);

    Napi::Array returnArr = Napi::Array::New(env, result.size());
    for (int i = 0; i < result.size(); i++) {
        returnArr.Set(i, result[i]);
    }
    return returnArr;
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
  exports.Set(Napi::String::New(env, "jaroWinklerDistance"),
       Napi::Function::New(env, JaroWinklerDistanceObjectMethod)); // Named Export 1

  exports.Set(Napi::String::New(env, "objectJaroWinklerSearch"),
           Napi::Function::New(env, ObjectJaroWinklerSearchMethod)); // Named Export 2

  return exports;
}
NODE_API_MODULE(testaddon, Init)
