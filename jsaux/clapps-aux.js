// This is an example of an add-on JavaScript script or library that can
// be initialized along with the rest of Clarity.

console.log("Hello from Clarity Apps Demo.js!");

function clappsfoo(x) {
    console.log("FOO = " + x);
}

function makeEl(obj, method) {
    return function(ev) {
        method(obj);
    }
}