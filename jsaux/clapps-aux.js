// This is an example of an add-on JavaScript script or library that can
// be initialized along with the rest of Clarity.

console.log("Hello from Clarity Apps Demo auxilliary JavaScript file!");



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




function elgMakeNewReactionRuleButtonClicked(beaker) {
    return function (e) {
        //jsProxyNode.clarityNode.updateNodeFromDom();
        //Module.ClarityNode.updateNodeFromDomById(jsProxyNode.id)
        console.log(beaker)
        beaker.makeNewReactionRule();
        //   if (jsProxyNode.boundField != undefined)
        //     jsProxyNode.nodelog("MakeNewReactionRuleButtonClicked Clicked: " + jsProxyNode.domElement[jsProxyNode.boundField]);
        //   else
        //     jsProxyNode.nodelog("MakeNewReactionRuleButtonClicked Clicked, no boundField");
    }
}


function makeNewReactionRule_el(ev) {
    console.log("Event: " + ev);
    console.log("makeNewReactionRule_el()");
}