# JWFSearch

Node package written as an addon in CPP. Uses JaroWinkler to fuzzySearch an array.

Webpack (Vite, Vue, NextJS) Compatibility requires a loader.

```js
// next config
const nextConfig = {
  // configure loader for jwfsearch
  webpack: (config) => {
    config.externals = [...config.externals, { jwfsearch: 'jwfsearch' }];
    return config;
  },
};
```

```
npm i jwfsearch
```

```js
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
threshold : float, value between 0 and 1. 0 matches everything. 1 matches only the exact string

result : string[], Results that meet the threshold, returned in order of best match -> worst match
*/

let fuzzyResults = fuzzySearch.array(search, states, fuzz);
// ['Alabama']

fuzz = 0.65;
search = 'ne jerksy';
fuzzyResults = fuzzySearch.array(search, states,fuzz);
// ['New Jersey','New York']

fuzz = 0.7; // higher value removes less likely matches
search = 'ne jerksy';
fuzzyResults = fuzzySearch.array(search, states,fuzz);
// ['New Jersey']
```

```js
import jwf from 'jwfsearch';

const documents = [
  {
    id:0,
    color:"Purple"
  },
  {
    id:1,
    color:"Purple"
  },
  {
    id:2,
    color:"Purple"
  },
  {
    id:3,
    color:"Purple"
  },


]

const results = jwf.object("Prple",documents,['color'],0.65);


```

![image](https://github.com/brandonetter/FuzzySearch-CPP-Addon/assets/4108484/9c7558ad-2ea3-46e1-999c-c8728e8425a6)

Performs 10x faster than Javascript version
