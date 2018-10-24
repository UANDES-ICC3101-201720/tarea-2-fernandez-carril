# virtmem
Operating Systems and Networks - Virtual Memory Assignment (Tarea 2)

Ignacio Carril
Santiago Fernández

En esta entrega debíamos realizar algoritmos de reemplazo de páginas, en este caso 3 algoritmos diferentes: FIFO, Random y un custom.
Nuestro algoritmo custom es una simplificación del algoritmo FIFO, no es tan optimo como el algoritmo LRU, pero es más fácil de implementar, y mejora ligeralmente el resultado comparado con el algoritmo FIFO.

Dentro de los resultados obtuvimos para SCAN:
(100 pages, 3 frames)

Random: PageFaults = 1100 DiscWrites = 1100 DiscReads = 1100
FIFO: PageFaults = 1090 DiscWrites = 1090 DiscReads = 1090 
Custom: PageFaults = 1100 DiscWrites = 1100 DiscReads = 1100

SORT:
(100 pages, 3 frames)

Random: PageFaults = 1313 DiscWrites = 1313 DiscReads = 1313
Custom: PageFaults = 1313 DiscWrites = 1313 DiscReads = 1313
FIFO:: PageFaults = 1373 DiscWrites = 1373 DiscReads = 1373

FOCUS:
(100 pages, 3 frames)

FIFO: PageFaults = 297 DiscWrites = 297 DiscReads = 297
Random: PageFaults = 297 DiscWrites = 297 DiscReads = 297
Custom: PageFaults = 297 DiscWrites = 297 DiscReads = 297

Como podemos ver los resultados no varían mucho entre ellos. Es de esperarse que estos 3 algoritmos que no son muy eficientes tengan resultados similares o directamente iguales. 
