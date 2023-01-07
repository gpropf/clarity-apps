

//console.log("clapps-aux.js loaded!");

/**
 * Probably could use support for the actual event instead of just throwing it away.
 * 
 * @param {*} obj An object of some type
 * @param {*} method The method to call on that object within the event listener
 * @returns an event listener that runs obj.method.
 */
function makeEl(obj, method) {
    return function (ev) {
        method(obj);
    }
}

function beakerIterate(beaker) {
    return function () {
        console.log("Iterating beaker...");
        beaker.iterate();
    }
}

function elgBeakerIterate(beaker) {
    return function (e) {
        console.log(beaker)
        beaker.iterate();
    }
}

function elgMakeNewReactionRuleButtonClicked(beaker) {
    return function (e) {
        console.log(beaker)
        beaker.makeNewReactionRule();
    }
}

function makeNewReactionRule_el(ev) {
    console.log("Event: " + ev);
    console.log("makeNewReactionRule_el()");
}

function colorChange_el(ev) {
    console.log("Event: " + ev);
    console.log("colorChange_el()");
}

// function setTicker(stickWorld) {
//     setInterval(() => {stickWorld.tick()}, 500);
// }

// function setTickerSWNode(stickWorldNode) {
//     setInterval(() => {stickWorldNode.tick()}, 100);
// }


