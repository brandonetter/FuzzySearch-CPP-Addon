# JWFSearch

Node package written as an addon in CPP. Uses JaroWinkler to fuzzySearch an array.

```
npm i jwfsearch
```

```
const fuzzySearch = require('jwfsearch');

let states = [
    'Alabama', 'Alaska', 'Arizona', 'Arkansas', 'California', 'Colorado',
    'Connecticut', 'Delaware', 'Florida', 'Georgia', 'Hawaii', 'Idaho',
    'Illinois', 'Indiana', 'Iowa', 'Kansas', 'Kentucky', 'Louisiana',
    'Maine', 'Maryland', 'Massachusetts', 'Michigan', 'Minnesota',
    'Mississippi', 'Missouri', 'Montana', 'Nebraska', 'Nevada',
    'New Hampshire', 'New Jersey', 'New Mexico', 'New York',
    'North Carolina', 'North Dakota', 'Ohio', 'Oklahoma', 'Oregon',
    'Pennsylvania', 'Rhode Island', 'South Carolina', 'South Dakota',
    'Tennessee', 'Texas', 'Utah', 'Vermont', 'Virginia', 'Washington',
    'West Virginia', 'Wisconsin', 'Wyoming'
];

let fuzz = 0.78;
let search = 'lamaba';
/*
searchTerm : string, The word to match with each element of the array
searchArray : string[], The array to search through
threshold : float, value between 0 and 1. 0 matches nothing. 1 matches only the exact string

result : string[], Results that meet the threshold, returned in order of best match -> worst match
*/

let fuzzyResults = fuzzySearch(search, states, fuzz);
// ['Alabama']

fuzz = 0.65;
search = 'ne jerksy';
fuzzyResults = fuzzySearch(search, states,fuzz);
// ['New Jersey','New York']

fuzz = 0.7; // higher value removes less likely matches
search = 'ne jerksy';
fuzzyResults = fuzzySearch(search, states,fuzz);
// ['New Jersey']


