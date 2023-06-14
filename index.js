
const testAddon = require("./build/Release/testaddon.node")
console.log("addon", testAddon)
console.log("addon", testAddon.hello("a"))
console.log("addon", testAddon.sort(["d", "b", "c"]))
console.log("addon", testAddon.fuzzySearch("hello", ["halo", "bello", "helli", "halp"], 0.7))
module.exports = testAddon
