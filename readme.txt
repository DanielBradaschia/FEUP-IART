Compilação e Execução
    -Abra um terminal no diretório do ficheiro
    -execute os comandos:
        g++ -o projeto projeto.cpp
        "EM UNIX" -> ./projeto
        "Em Windows" .\projeto
    -forneça o caminho do arquivo que deseja testar
    - o resultado será guardado em slideshow.txt


Exemplo de formatação do arquivo de entrada
Input file              |        Description
--------------------------------------------------------------------------------------
4                       |        The collection has 4 photos
H 3 cat beach sun       |        Photo 0 is horizontal and has tags [cat, beach, sun]
V 2 selfie smile        |        Photo 1 is vertical and has tags [selfie, smile]
V 2 garden selfie       |        Photo 2 is vertical and has tags [garden, selfie]
H 2 garden cat          |        Photo 3 is horizontal and has tags [garden, cat]   


Exemplo de formatação do arquivo de saída
3                       |       The slideshow has 3 slides
0                       |       First slide contains photo 0
3                       |       Second slide contains photo 3
1 2                     |       Third slide contains photos 1 and 2


Nota: foi fornecido um diretório test com ficheiros para teste.
