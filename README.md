## node插件

##### 1. 安装环境
`npm install -g node-gyp`

##### 2. 配置
`node-gyp configure`

##### 3. 生成插件
执行`node-gyp build`命令后，在`build/Release/`目录下会生成`faker.node`文件

##### 4. 测试代码
`node test.js`
