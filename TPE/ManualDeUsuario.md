# Manual de usuario #

## Compilación
Este kernel debe correrse a través del emulador qemu y se debe compilar desde docker a través del contenedor 'agodio/itba-so:1.0', para ello una vez abierta la terminal de dicha aplicación se deben seguir los siguientes pasos:

1. La primera vez que uno accede debe ejecutar `cd Toolchain` y luego `make all`.

2. Luego resta compilar todo el proyecto desde el directorio principal, para ello si todavia se encuentra en `Toolchain` ejecute `cd ..` y luego `make all` para compilar y linkeditar todo el proyecto. 

3. Último, debe ejecutar el archivo `run.sh`, que contiene el script con el comando correspondiente para correr al sistema a través de qemu.


## Terminal

Una vez ejecutado el run.sh, se abrirá una ventana con una terminal donde usted dispondrá de los siguientes comandos:

- ***help***: Despliega en pantalla todos los comandos disponibles para el usuario con su respectiva descripción.

- ***inforeg***: Muestra en pantalla el valor de los registros, si es que se guardó previamente. Para guardar el estado de los registros en un momento específico, simplemente debe presionar la tecla `Shift`.

- ***printmem***: Realiza un dump de 32 bytes de una zona de memoria que recibe por parámetro. 

- ***time***: Despliega la hora y fecha actual.

- ***clear***: Limpia la pantalla, eliminando los comandos anteriores dejando a la terminal en su estado inicial, lista para recibir un nuevo comando.

- ***divzero***: Verifica el correcto funcionamiento de la excepción de división por cero.

- ***invalidopcode***: Verifica el correcto funcionamiento de la excepción de código de operación invalido.

- ***gamemode***: Divide la pantalla en cuatro: en una zona muestra la hora en formato hh:mm:ss, en otra hay un cronómetro funcional y en las restantes se puede jugar al sudoku y al ahorcado respectivamente.


## Gamemode

Si usted ejecuta el comando `gamemode`, dispondrá de las siguientes funcionalidades:

- ***Reloj***: Arriba a la izquierda observará un reloj en tiempo real en formato hh:mm:ss.

- ***Cronómetro***: Por debajo del reloj, se encontrara con un cronómetro, su funcionamiento consiste en lo siguiente:
	- `Ctrl` ==> Comienza / pausa / reanuda el cronómetro.
	- `Alt` ==> Reinicia.

- ***Ahorcado***: Por la parte inferior izquierda, usted podrá jugar un juego de ahorcado. Dispondrá de 7 vidas y para jugar simplemente debera pulsar la letra que desee. Si es correcta, aparecerá en la palabra. Si no, se le descontará una vida y la letra se sumará a las ya utilizadas, por lo que no perderá otra vida si vuelve a ingresar la misma.
Si termina el juego, ya sea porque adivinó la palabra o porque perdió, presionando la tecla `R` podrá jugar de nuevo.

- ***Sudoku***: Por la zona derecha, tambien se podra jugar a un sudoku de 9x9, para ello usted debe ingresar la columna (1-9), luego la fila (1-9) y por último el número a ingresar. Si el número es válido, figurará al instante en el sudoku actualizado, si no lo es se reiniciará el proceso.
	Si termina el juego, usted podrá reiniciarlo pulsando al tecla `0`.

Si usted desea salir del gamemode, pulse la tecla `Esc`.

## Syscalls

[0] char ***sys_getChar***();
Devuelve un caracter ingresado por el usuario o 0 en su defecto

[1] extern void ***sys_putCharWC***(char c, unsigned char colorCode);
Ubica el caracter 'c' en la posicion actual de la pantalla con el color indicado en 'colorCode'

[2] int ***sys_getTime***(int descriptor);
Devuelve el campo de la fecha/hora actual indicado en 'descriptor'

[3] void ***sys_clear***();
Elimina todo el contenido de la pantalla y posiciona el prompt al comienzo de la misma.

[4] void ***sys_saveRegs***();
Crea un backup de los registros para luego imprimirlos cuando se llame `inforeg`

[5] void ***sys_PrintMem***(char * address);
Imprime en pantalla el contenido de 32 bytes de memoria a partir de la direccion 'address'.

[6] void ***sys_printCharInPos***(char c, int row, int col, unsigned char colorCode);
Imprime el caracter 'c' en la fila 'row' y columna 'col' de la pantalla, teniendo en cuenta que la misma tiene un tamaño de 25x80. Ademas, lo hace en el color indicado por 'colorCode'

[7] unsigned long ***sys_getDeciseconds***();
Retorna los decisegundos desde que se inicio el sistema

[8] void ***sys_getRegistersInfo***();
Imprime en pantalla cada registro del procesador con su contenido


