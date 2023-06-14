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
  exports.Set(Napi::String::New(env, "fuzzySearch"),
              Napi::Function::New(env, JaroWinklerDistanceMethod));
    return exports;
}
NODE_API_MODULE(testaddon, Init)
