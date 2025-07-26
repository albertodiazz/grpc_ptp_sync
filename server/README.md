# OBJETIVO

Medir la latencia total (ida y vuelta) y ver la diferencia entre el tiempo del cliente y el del servidor.

## Flujo:

    El cliente toma su timestamp (ts1) justo antes de hacer el rpc.SendRequest(...).

    El servidor recibe la petición y toma su timestamp (ts2).

    El servidor responde con ts2.

    El cliente recibe la respuesta y toma otro timestamp (ts3).

## Resultado que obtienes:

    ts3 - ts1 → Latencia ida/vuelta.

    ts2 - ts1 → Diferencia de reloj entre cliente y servidor más media latencia ida.

    ts2 puede ser comparado contra el reloj del cliente para estimar si están desfasados.

## Formulas

Sin los 3 tiempos no puedes sacar el offset, por eso es necesario el tiempo del servidor.

### OFSET
``` 
ofset -> t2 - (t1 + t3 / 2)
```

Si no tienes ambos solo puedes calcular el roundtrip time, que es la ida y vuelta. Mas no el desfase real entre los relojes ya que no tienes el dato del reloj.

### RoundTrip
``` 
roundtrip -> t3 - t1
```

## NOTAS

Comando que se corre solo una vez, al menos que se realicen cambios en el `conanfile.txt` que es donde manejamos nuestras librerias. 
```
conan install . --output-folder=build --build=missing
``` 

Comando que se ocupa solo al configurar el proyecto o cambias el `CMakeLists.txt` o quieres cambiar el toolchain o el modo ya sea a Debug Release o volviste a cambiar el conan file y por lo tanto corriste el anterior comando el conan install.
```
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
``` 

Con el siguiente comando vemos que ocupa nuestra instalacion tipo los lib que ocupamos en el CMAKE
``` 
pacman -Ql protobuf
``` 

```
source conanbuildenv-release-x86_64.sh

protoc -I ../proto --grpc_out=../proto --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin ../proto/test.proto

protoc -I ../proto --cpp_out=../proto ../proto/test.proto
```

