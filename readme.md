#snmp-collector

### dep

* gcc >4.8.x, support c++11
* net-snmp > 5.7.1
* using https://github.com/nlohmann/json - JSON for Modern C++

### Compile

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j8
#make install
```