# CppJieba [English](README_EN.md)

[![CMake](https://github.com/yanyiwu/cppjieba/actions/workflows/cmake.yml/badge.svg)](https://github.com/yanyiwu/cppjieba/actions/workflows/cmake.yml)
[![Author](https://img.shields.io/badge/author-@yanyiwu-blue.svg?style=flat)](http://yanyiwu.com/) 
[![Platform](https://img.shields.io/badge/platform-Linux,%20OS%20X,%20Windows-green.svg?style=flat)](https://github.com/yanyiwu/cppjieba)
[![Performance](https://img.shields.io/badge/performance-excellent-brightgreen.svg?style=flat)](http://yanyiwu.com/work/2015/06/14/jieba-series-performance-test.html) 
[![Tag](https://img.shields.io/github/v/tag/yanyiwu/cppjieba.svg)](https://github.com/yanyiwu/cppjieba/releases)
[![License](https://img.shields.io/badge/license-MIT-yellow.svg?style=flat)](http://yanyiwu.mit-license.org)
[![Build status](https://ci.appveyor.com/api/projects/status/wl30fjnm2rhft6ta/branch/master?svg=true)](https://ci.appveyor.com/project/yanyiwu/cppjieba/branch/master)

## 

CppJieba"(Jieba)"C++

## 

+ `include/cppjieba/*.hpp``include`
+ `utf8`
+ (utf8)
+ '|'';'
+  `Linux` , `Mac OSX`, `Windows` 

## 

### 

* `g++ (version >= 4.1 is recommended) or clang++`;
* `cmake (version >= 2.6 is recommended)`;

### 

```sh
git clone --depth=10 --branch=master git://github.com/yanyiwu/cppjieba.git
cd cppjieba
mkdir build
cd build
cmake ..
make
```

():

```
make test
```

## Demo

```
./demo
```



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

 `test/demo.cpp`.

### 

**MPSegment**

Output:
```

///


//////


///////////

```

**HMMSegment**

```

///


/////


////////////////

```

**MixSegment**

```

///


/////


//////////

```

**FullSegment**

```

//////


//////


////////////////////

```

**QuerySegment**

```

//////


/////


///////////////////////

```

MP,HMM,Mix  

MixMPHMM""

Full

QueryMixFull

### 

`dict/user.dict.utf8`

:

```
//////
```

:

```
/////
```

### 

```
CEO
["CEO:11.7392", ":10.8562", ":10.6426", ":10.0089", ":9.49396"]
```

 `test/demo.cpp`.

### 

```
CEO
[":r", ":v", ":n", ":n", ":n", ":n", ":uj", ":x", ":v", ":m", ":x", ":r", ":d", ":v", ":v", ":nr", ":x", ":t", "CEO:eng", ":x", ":v", ":n", ":n", ":x"]
```

 `test/demo.cpp`.


(`dict/user.dict.utf8`)

```
 nz
```



```
[":r", ":v", ":nz", ":n", ":n", ":n", ":n", ":n", ":uj", ":x", ":v", ":m", ":x", ":r", ":d", ":v", ":v", ":nr", ":x", ":t", ":f", ":n", ":x", ":v", "CEO:eng", ":x", ":v", ":x", ":x", ":v", ":n", ":n", ":x"]
```

## 

+ [dict.367W.utf8] iLife(562193561 at qq.com)

## 

+ [GoJieba] go
+ [NodeJieba] Node.js 
+ [simhash] 
+ [exjieba] Erlang 
+ [jiebaR] R
+ [cjieba] C
+ [jieba_rb] Ruby 
+ [iosjieba] iOS 
+ [SqlJieba] MySQL 
+ [pg_jieba] PostgreSQL 
+ [simple] SQLite3 FTS5 
+ [gitbook-plugin-search-pro]  gitbook 
+ [ngx_http_cppjieba_module] Nginx 
+ [cppjiebapy]  [jannson]  python  [cppjiebapy],  [cppjiebapy_discussion] .
+ [cppjieba-py]  [bung87]  pybind11  python ,jieba
+ [KeywordServer] 50
+ [cppjieba-server] CppJieba HTTP 
+ [phpjieba] php
+ [perl5-jieba] Perl
+ [jieba-dlang] D  Deimos Bindings

## 

[Web-Demo](http://cppjieba-webdemo.herokuapp.com/)
(chrome)

## 

[Jieba]

## Sponsorship

[![sponsorship](http://images.gitads.io/cppjieba)](https://tracking.gitads.io/?campaign=gitads&repo=cppjieba&redirect=gitads.io)

## Contributors

### Code Contributors

This project exists thanks to all the people who contribute.
<a href="https://github.com/yanyiwu/cppjieba/graphs/contributors"><img src="https://opencollective.com/cppjieba/contributors.svg?width=890&button=false" /></a>

[GoJieba]:https://github.com/yanyiwu/gojieba
[CppJieba]:https://github.com/yanyiwu/cppjieba
[jannson]:https://github.com/jannson
[cppjiebapy]:https://github.com/jannson/cppjiebapy
[bung87]:https://github.com/bung87
[cppjieba-py]:https://github.com/bung87/cppjieba-py
[cppjiebapy_discussion]:https://github.com/yanyiwu/cppjieba/issues/1
[NodeJieba]:https://github.com/yanyiwu/nodejieba
[jiebaR]:https://github.com/qinwf/jiebaR
[simhash]:https://github.com/yanyiwu/simhash
[]:https://github.com/yanyiwu/cppjieba/wiki/CppJieba%E4%BB%A3%E7%A0%81%E8%AF%A6%E8%A7%A3
[issue25]:https://github.com/yanyiwu/cppjieba/issues/25
[exjieba]:https://github.com/falood/exjieba
[KeywordServer]:https://github.com/yanyiwu/keyword_server
[ngx_http_cppjieba_module]:https://github.com/yanyiwu/ngx_http_cppjieba_module
[dict.367W.utf8]:https://github.com/qinwf/BigDict
[cjieba]:http://github.com/yanyiwu/cjieba
[jieba_rb]:https://github.com/altkatz/jieba_rb
[iosjieba]:https://github.com/yanyiwu/iosjieba
[SqlJieba]:https://github.com/yanyiwu/sqljieba
[Jieba]:http://yanyiwu.com/work/2015/06/14/jieba-series-performance-test.html
[pg_jieba]:https://github.com/jaiminpan/pg_jieba
[gitbook-plugin-search-pro]:https://plugins.gitbook.com/plugin/search-pro
[cppjieba-server]:https://github.com/yanyiwu/cppjieba-server
[phpjieba]:https://github.com/jonnywang/phpjieba
[perl5-jieba]:https://metacpan.org/pod/distribution/Lingua-ZH-Jieba/lib/Lingua/ZH/Jieba.pod
[jieba-dlang]:https://github.com/shove70/jieba
[simple]:https://github.com/wangfenjin/simple


