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

- ***mem***: Imprime el estado de la memoria.

- ***ps***: Imprime la lista de todos los procesos en ejecución con sus propiedades.

- ***sleep***: Retrasa el funcionamiento de la terminal una cantidad especifica de segundos.

- ***loop***: Imprime el ID del proceso actual con un saludo cada una cantidad de segundos.

- ***block***: Cambia el estado de un proceso entre BLOCKED y READY dado su ID.

- ***kill***: Mata un proceso dado su ID.

- ***nice***: Cambia la prioridad de un proceso dado su ID y la nueva prioridad.

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

[9] void ***sys_ps***();
Imprime una lista con todos los procesos en ejecución

[10] unsigned long long ***sys_getPid***();
Retorna el Process ID del proceso actual

[11] void ***sys_togglePsState***(unsigned long long pid);
Cambia el estado de un proceso entre bloqueado y listo dado su ID

[12] int ***sys_getSeconds***();
Retorna los segundos desde que se inicio el sistema

[13] void ***sys_mem***();
Imprime el estado actual de la memoria
