#IPP-labyrinth
Program reading coded input of k-dimensional labyrinth consisting of k-dimensional cubes (each cube can be full or empty) and prints length of the shortest path from start to end (also given in input) if exists and "NO WAY" if there is no such path.

Assignment in polish available below:
> # Definicje
> Labirynt jest zawarty w niepustym przedziale k-wymiarowym (prostopadłościanie k-wymiarowym) składającym się z k-wymiarowych kostek jednostkowych. Każda z takich kostek może być wypełniona, tworząc ściany labiryntu, lub pusta, tworząc przestrzeń, w której można się poruszać. Po labiryncie można się poruszać, przechodząc pomiędzy pustymi kostkami stykającymi się ścianą (k−1)-wymiarową.
> # Dane wejściowe
> Program czyta dane ze standardowego wejścia. Poprawne dane składają się z czterech linii tekstu. Trzy początkowe linie zawierają po k liczb całkowitych z przedziału od 1 do SIZE_MAX, gdzie k jest dodatnią liczbą całkowitą określającą wymiar labiryntu. Są to odpowiednio:
> * $n_1, n_2, n_3,...,n_k$
> * $x_1, x_2, x_3,...,x_k$
> * $y_1, y_2, y_3,...,y_k$
> 
> gdzie liczby $n_i$ określają rozmiary labiryntu w poszczególnych wymiarach, a liczby $x_i$ i $y_i$ określają odpowiednio współrzędne pozycji początkowej i końcowej.
> Czwarta linia zawiera liczbę całkowitą opisującą położenia ścian labiryntu. W rozwinięciu dwójkowym tej liczby bit
> $(z_1 -1)+(z_2-1)n_1+(z_3-1)n_1n_2+...+(z_k-1)n_1n_2...n_{k-1}$ określa kostkę o współrzędnych $(z_1,z_2,...,z_k)$. Jeśli bit ten ma wartość 0, to kostka jest pusta, a jeśli ma wartość 1, to kostka jest wypełniona.
>
> Liczba w czwartej linii może być podana na dwa sposoby:
> liczba szesnastkowa zaczynająca się kombinacją znaków 0x, zapisana z użyciem cyfr 0, 1, …, 9, a, b, …, f, A, B, … F.
znak R, po którym występuje pięć zapisanych przy podstawie 10 liczb całkowitych z przedziału od 0 do UINT32_MAX: $a, b, m, r, s_0$, używanych w niżej opisany sposób, przy czym nakładamy dodatkowy warunek, że liczba m nie jest zerem.
Wyliczamy liczby $s_1, s_2,...s_r$ ze wzoru $s_i=(as_i−1+b)\mod m$. Wyliczamy reszty $w_i=s_i\mod n_1n_2…n_k$. Liczba opisująca położenie ścian labiryntu ma w rozwinięciu dwójkowym ustawiony bit numer j, jeśli istnieje taki indeks i, że $j\mod 2^{32}=w_i$.
>
> Liczby mogą być poprzedzone wiodącymi zerami. Liczby występujące w jednej linii rozdzielone są dowolną liczbą białych znaków. Między znakiem R a pierwszą liczbą może nie być odstępu lub może być dowolna liczba białych znaków. Na początku i na końcu każdej linii może występować dowolna liczba białych znaków.
> # Opis działania programu
> Program czyta dane, a następnie, jeśli są poprawne, wyznacza długość najkrótszej drogi od pozycji początkowej do pozycji końcowej. Program wypisuje na standardowe wyjście jedną linię (zakończoną znakiem przejścia do nowej linii, czyli w języku C znakiem \n o kodzie ASCII 10) zawierającą długość znalezionej drogi (bez zer wiodących) lub komunikat NO WAY, jeśli nie ma drogi. Program kończy działanie kodem 0. Program powinien jawnie zwolnić zaalokowaną pamięć.
> # Obsługa błędów
>Jeśli dane wejściowe są niepoprawne, to program wypisuje na standardowe wyjście diagnostyczne jedną linię (zakończoną znakiem przejścia do nowej linii) zawierającą komunikat ERROR L, gdzie L jest liczbą całkowitą z przedziału od 1 do 5. Jest to numer pierwszej linii związanej z błędem. Jeśli wystąpił nienaprawialny błąd, np. funkcja biblioteczna zakończyła się niepowodzeniem, zabrakło pamięci, nie da się wyznaczyć wyniku, to program wypisuje na standardowe wyjście diagnostyczne jedną linię (zakończoną znakiem przejścia do nowej linii) zawierającą komunikat ERROR 0. Program kończy działanie kodem 1. Program przed zakończeniem powinien jawnie zwolnić zaalokowaną pamięć, chyba że wystąpił błąd alokacji pamięci.