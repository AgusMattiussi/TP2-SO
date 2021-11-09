## Manual de usuario ##

# Compilación
Este kernel debe correrse a través del emulador qemu y se debe compilar desde docker a través del contenedor 'agodio/itba-so:1.0', para ello una vez abierta la terminal de dicha aplicación se deben seguir los siguientes pasos:

1. La primera vez que uno accede debe ejecutar `cd Toolchain` y luego `make all`.

2. Luego resta compilar todo el proyecto desde el directorio principal, para ello si todavia se encuentra en `Toolchain` ejecute `cd ..` y luego `make all` para compilar y linkeditar todo el proyecto. 

3. Último, debe ejecutar el archivo `run.sh`, que contiene el script con el comando correspondiente para correr al sistema a través de qemu.


# Terminal

Una vez ejecutado el run.sh, se abrirá una ventana con una terminal donde usted dispondrá de los siguientes comandos:

***help***: Despliega en pantalla todos los comandos disponibles para el usuario con su respectiva descripción.

***inforeg***: Muestra en pantalla el valor de los registros.

***printmem***: Realiza un dump de 32 bytes de una zona de memoria que recibe por parámetro. 

***time***: Despliega la hora y fecha actual.

***clear***: Limpia la pantalla, eliminando los comandos anteriores dejando a la terminal en su estado inicial, lista para recibir un nuevo comando.

***divzero***: Verifica el correcto funcionamiento de la excepción de división por cero.

***invalidopcode***: Verifica el correcto funcionamiento de la excepción de código de operación invalido.

***gamemode***: Divide la pantalla en cuatro: en una zona muestra la hora en formato hh:mm:ss, en otra hay un cronómetro funcional y en las restantes se puede jugar al sudoku y al ahorcado respectivamente.


# Gamemode

Si usted ejecuto el comando `gamemode`, usted dispondrá de las siguientes funcionalidades:

Reloj: Arriba a la izquierda observara un reloj en tiempo real en formato hh:mm:ss.

***Cronómetro***: Por debajo del reloj, se encontrara con un cronómetro, su funcionamiento consiste en lo siguiente:
	- `Ctrl` ==> Comienza / pausa / reanuda el cronómetro.
	- `Alt` ==> Reinicia.

***Ahorcado***: Por la parte inferior izquierda, usted podrá jugar un juego de ahorcado. Dispondrá de 7 vidas y para jugar simplemente debera pulsar la letra que desee. Si es correcta aparecerá en la palabra, si no se le descontará una vida y la letra se sumará a las ya utilizadas, por lo que no perderá otra vida si vuelve a ingresar la misma.
Si termina el juego, ya sea porque adivinó la palabra o porque perdió, presionando la tecla `R` podrá jugar de nuevo.

***Sudoku***: Por la zona derecha, tambien se podra jugar a un sudoku de 9x9, para ello usted debe ingresar el número que desea escribir, luego se le pedirá la fila y la columna para este. Si el número es válido, figurará al instante en el sudoku actualizado, si no lo es se reiniciará el proceso.
	Si termina el juego, usted podrá reiniciarlo pulsando al tecla `0`.

Si usted desea salir del gamemode pulse la tecla `Esc`.



