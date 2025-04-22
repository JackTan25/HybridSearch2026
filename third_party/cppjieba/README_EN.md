# CppJieba [](README.md)

[![Build Status](https://travis-ci.org/yanyiwu/cppjieba.png?branch=master)](https://travis-ci.org/yanyiwu/cppjieba) 
[![Author](https://img.shields.io/badge/author-@yanyiwu-blue.svg?style=flat)](http://yanyiwu.com/) 
[![Platform](https://img.shields.io/badge/platform-Linux,%20OS%20X,%20Windows-green.svg?style=flat)](https://github.com/yanyiwu/cppjieba)
[![Performance](https://img.shields.io/badge/performance-excellent-brightgreen.svg?style=flat)](http://yanyiwu.com/work/2015/06/14/jieba-series-performance-test.html) 
[![License](https://img.shields.io/badge/license-MIT-yellow.svg?style=flat)](http://yanyiwu.mit-license.org)
[![Build status](https://ci.appveyor.com/api/projects/status/wl30fjnm2rhft6ta/branch/master?svg=true)](https://ci.appveyor.com/project/yanyiwu/cppjieba/branch/master)

[![logo](http://7viirv.com1.z0.glb.clouddn.com/CppJiebaLogo-v1.png)](https://github.com/yanyiwu/cppjieba)

## Introduction

The Jieba Chinese Word Segmentation Implemented By C++ .

## Usage 

### Dependencies

+ `g++ (version >= 4.1 is recommended) or clang++`;
+ `cmake (version >= 2.6 is recommended)`;

### Download & Compile

```sh
git clone --depth=10 --branch=master git://github.com/yanyiwu/cppjieba.git
cd cppjieba
mkdir build
cd build
cmake ..
make
```

### Unit Testing

```
make test
```

## Demo

```
./demo
```

Output:

```
[demo] Cut With HMM
/////
[demo] Cut Without HMM
//////

[demo] CutAll
//////

[demo] CutForSearch
//////////////////
[demo] Insert User Word
/

[demo] CutForSearch Word With Offset
[{"word": "", "offset": 0}, {"word": "", "offset": 6}, {"word": "", "offset": 12}, {"word": "", "offset": 18}, {"word": "", "offset": 21}, {"word": "", "offset": 27}, {"word": "", "offset": 30}, {"word": "", "offset": 27}, {"word": "", "offset": 21}, {"word": "", "offset": 36}, {"word": "", "offset": 36}, {"word": "", "offset": 45}, {"word": "", "offset": 48}, {"word": "", "offset": 51}, {"word": "", "offset": 54}, {"word": "", "offset": 60}, {"word": "", "offset": 66}, {"word": "", "offset": 54}, {"word": "", "offset": 72}]
[demo] Tagging
CEO
[:r, :v, :n, :n, :n, :n, :uj, :x, :v, :m, :x, :r, :d, :v, :v, :nr, :x, :t, CEO:eng, :x, :v, :n, :n, :x]
[demo] Keyword Extraction
CEO
[{"word": "CEO", "offset": [93], "weight": 11.7392}, {"word": "", "offset": [72], "weight": 10.8562}, {"word": "", "offset": [78], "weight": 10.6426}, {"word": "", "offset": [21], "weight": 10.0089}, {"word": "", "offset": [111], "weight": 9.49396}]
```

Please see details in `test/demo.cpp`.

## Cases

+ [GoJieba] 
+ [NodeJieba]
+ [simhash]
+ [exjieba]
+ [jiebaR]
+ [cjieba]
+ [jieba_rb]
+ [iosjieba]
+ [SqlJieba]
+ [pg_jieba]
+ [ngx_http_cppjieba_module]
+ [gitbook-plugin-search-pro]
+ [cppjieba-server]
+ [perl5-jieba]
+ [jieba-dlang]

## Contact

+ Email: `i@yanyiwu.com`
+ QQ: 64162451
+ WeChat: ![image](http://7viirv.com1.z0.glb.clouddn.com/5a7d1b5c0d_yanyiwu_personal_qrcodes.jpg)

[GoJieba]:https://github.com/yanyiwu/gojieba
[CppJieba]:https://github.com/yanyiwu/cppjieba
[jannson]:https://github.com/jannson
[cppjiebapy]:https://github.com/jannson/cppjiebapy
[cppjiebapy_discussion]:https://github.com/yanyiwu/cppjieba/issues/1
[NodeJieba]:https://github.com/yanyiwu/nodejieba
[jiebaR]:https://github.com/qinwf/jiebaR
[simhash]:https://github.com/yanyiwu/simhash
[exjieba]:https://github.com/falood/exjieba
[cjieba]:http://github.com/yanyiwu/cjieba
[jieba_rb]:https://github.com/altkatz/jieba_rb
[iosjieba]:https://github.com/yanyiwu/iosjieba
[SqlJieba]:https://github.com/yanyiwu/sqljieba
[pg_jieba]:https://github.com/jaiminpan/pg_jieba
[gitbook-plugin-search-pro]:https://plugins.gitbook.com/plugin/search-pro
[cppjieba-server]:https://github.com/yanyiwu/cppjieba-server
[perl5-jieba]:https://metacpan.org/pod/distribution/Lingua-ZH-Jieba/lib/Lingua/ZH/Jieba.pod
[jieba-dlang]:https://github.com/shove70/jieba
