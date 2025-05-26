## node插件

##### 1. 安装环境
`npm install -g node-gyp`

##### 2. 配置
`node-gyp configure`

##### 3. 生成插件
执行`node-gyp build`命令后，在`build/Release/`目录下会生成`faker.node`文件

##### 4. 测试代码
`node test.js`



```js
const faker = require('./faker.node');

// 查看当前node的v8版本
console.log(faker.v8_version);

// 自定义的打印函数
faker.print("1111");

// 自定义绑定数据到底层，规避检测
o = {}
faker.set_data(o, 'cache', { 'name': 'val' });
console.log('cache: ', faker.get_data(o, 'cache'));
console.log("js层无法感知到cache的存在: ", o)
faker.del_data(o, 'cache');
console.log('cache: ', faker.get_data(o, 'cache'));

// 自定义的执行js函数()
faker.run_js("console.log('faker.run_js is called')", "VM1");
faker.run_js(`
    try{
        throw new Error("test");
    }catch(e){
        console.log(e.stack);
    }
    `, "https://www.test.com/test.js");

// 自定义的执行js文件函数
faker.run_js_file("./temp.js", "VM2");
// faker.run_js_file("./temp.js", "https://www.test.com/temp.js");


// 封装native函数
test = faker.native({
    name: "test",
    length: 0,
    cb: function () {
        console.log("native test is called:", ...arguments);
    }
});
console.log("test.toString():  ", test.toString());
test("hi faker.native");

// 生成HTMLAllCollection原型及document.all对象
let [document_all, HTMLAllCollection] = faker.new_document_all(function () {
    console.log("document.all is called:", ...arguments);
    return null;
});
document = { all: document_all };


Object.defineProperties(HTMLAllCollection.prototype, {
    length: {
        get: faker.native({
            name: "length", length: 0,
            cb: function () {
                return 3;
            }
        }), set: undefined, enumerable: true, configurable: true,
    },
    item: {
        value: faker.native({
            name: "item", length: 0,
            cb: function () {
                console.log("item is called: ", ...arguments);
                return null;
            }
        }), writable: true, enumerable: true, configurable: true,
    },
    namedItem: {
        value: faker.native({
            name: "namedItem", length: 0,
            cb: function () {
                console.log("namedItem is called: ", ...arguments);
                return null;
            }
        }), writable: true, enumerable: true, configurable: true,
    },
    constructor: { writable: true, enumerable: false, configurable: true, value: HTMLAllCollection },
    [Symbol.iterator]: {
        value: faker.native({
            name: "values", length: 0,
            cb: function* () {
                console.log("Symbol.iterator is called");
                for (const val of [1, 2, 3]) {
                    yield val;
                }
            }
        }), writable: true, enumerable: false, configurable: true,
    },
});

console.log("typeof document.all:", typeof document.all);
console.log("document.all.toString(): ", document.all.toString());
console.log("document.all.__proto__.toString(): ", document.all.__proto__.toString());
console.log("document.all instanceof HTMLAllCollection", document.all instanceof HTMLAllCollection);
console.log("HTMLAllCollection.toString(): ", HTMLAllCollection.toString());

console.log("document.all.length: ", document.all.length);
for (const val of document.all) {
    console.log("val: ", val);
}
console.log("document.all(0): ", document.all(0));
console.log("document.item(0): ", document.all.item(0));
console.log("document.namedItem('eleid'): ", document.all.namedItem('eleid'));

```