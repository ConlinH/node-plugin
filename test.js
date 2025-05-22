const faker = require('./faker');
document = {};
document.all = faker.DocumentAll();

HTMLAllCollection = function HTMLAllCollection() {};

document.all.__proto__ = HTMLAllCollection

console.log("typeof a  ", typeof document.all);
console.log("a===undefined  ", document.all===undefined);
console.log("a==undefined  ", document.all==undefined);
console.log("a!==undefined  ", document.all==undefined);
console.log("Object.getPrototypeOf(document.all)  ", Object.getPrototypeOf(document.all).toString());
console.log("a.__proto__  ", document.all.__proto__.toString());
