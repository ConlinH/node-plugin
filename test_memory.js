function printMemoryUsage(tag = '') {
    const used = process.memoryUsage();
    console.log(`\n--- Memory Usage ${tag} ---`);
    for (let key in used) {
        console.log(`${key}: ${(used[key] / 1024 / 1024).toFixed(2)} MB`);
    }
}
var faker = require("./faker.node");

for (let i = 0; i < 100000000; i++) {
    let demo = () => { console.log(i) }
    let [x1, x2] = faker.new_document_all()
    faker.set_data(x1, 'cb', (x) => { console.log(x) });
    // x1(i)
    if (i % 100000 === 0) {
        printMemoryUsage();
    }
    [x1, x2] = [null, null];
    demo = null;
}