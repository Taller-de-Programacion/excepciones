//
// Created by fedemgp on 7/4/22.
//

#include <gtest/gtest.h>
#include "DoubleBuffer.h"
#include "Stack.h"
#include "Date.h"

/*
 * Para capturar excepciones especificas, el catch de estas excepciones deben preceder a las de alguna mas genérica.
 * Es decir, si quiero capturar y tratar de forma particular std::logic_error (se lanza cuando hago un push a un stack
 * lleno) debo tener un catch antes de catchear una clase base como lo es std::exception.
 *
 * Ejercicio 1: Ver la jerarquia de las excepciones de la STL (https://stdcxx.apache.org/doc/stdlibug/18-2.html) ¿comop
 *              se arreglaría este test?
 *
 * Ejercicio 2: Es necesario tener un catch para cada excepcion que se pueda lanzar? agrega algún valor esto?
 */
TEST(Stack, pop_falla_si_no_hay_elementos) {
    try {
        Stack s(4);
        s.push(DummyFile(1));
        s.pop();
        s.pop();
    } catch(const std::logic_error &e) {
        // bad push
        std::cerr << e.what() << std::endl;
        FAIL() << "Se esperaba fallar por un mal pop";
    } catch (const std::runtime_error &e) {
        // Error en el constructor
        std::cerr << e.what() << std::endl;
        FAIL() << "Se esperaba fallar por un mal pop";
    } catch(const std::overflow_error &e) {
        // bad pop
        ASSERT_STRCASEEQ(e.what(), "Stack vacio");
        return;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        FAIL() << "Se esperaba fallar por un mal pop";
    }
    FAIL() << "Se esperaba que se lance una excepción";
}

TEST(DoubleBuffer, libera_los_recursos_si_el_constructor_falla) {
    /**
     * Ejercicio 3: Intentar correr este test usando valgrind, ¿qué sucede? ¿Cómo lo podemos arreglar?
     */
    try {
        DoubleBuffer buff(0x1000000000000); // En un mac usar 0x1000000000000
    } catch(const std::runtime_error &e) {
        ASSERT_STRCASEEQ(e.what(), "Error al reservar memoria para los buffers");
        return;
    }
    FAIL() << "Se esperaba que una excepción sea lanzada";
}
/*
 * El concepto de exception safety garantiza cierto comportamiento de la clase ante excepciones internas.
 *
 * Exception safe strong: si se lanza una excepción dentro de un método, el estado del objeto va a quedar inalterado.
 * Exception safe weak: Si se lanza la excepción, el estado del objeto va a ser válido, pero no exactamente igual al
 *                      original antes de que se lance la excepción.
 *
 * Ejercicio 4: estos setters cumplen ser exception safe strong? como lo corregiría para que sí lo sean.
 */
TEST(Date, set_day_es_exception_safe_strong) {
    Date d(2022, 02, 20);
    EXPECT_THROW(d.set_day(31), std::runtime_error);
    EXPECT_EQ(d, Date(2022, 02, 20));
}

TEST(Date, set_month_es_exception_safe_strong) {
    Date d(2022, 01, 30);
    EXPECT_THROW(d.set_month(2), std::runtime_error);
    EXPECT_EQ(d, Date(2022, 01, 30));
}

/*
 * Ejercicio 5: qué pasa si los uso en conjunto al set_day y set_month? puedo garantizar que su uso en conjunto
 *              cumpla con ser exception safe strong?
 */
TEST(Date, combinar_uso_setters_es_exception_safe_strong) {
    Date d(2022, 01, 30);
    d.set_day(31);
    EXPECT_THROW(d.set_month(2), std::runtime_error);
    EXPECT_EQ(d, Date(2022, 01, 30));
}

TEST(Stack, pop_retorna_ultimo_elemento_del_stack) {
    Stack stack(4);
    // Se está copiando el objeto acá? por qué sucede esto?
    stack.push(DummyFile(1));
    stack.push(DummyFile(2));
    EXPECT_EQ(stack.pop(), DummyFile(2));
}

TEST(Stack, pop_es_exception_safe_strong) {
    Stack s(4);
    s.push(DummyFile(1));
    s.push(DummyFile(2));
    /*
     * Cierro el ultimo file, haciendo que el file descriptor sea -1 para que el constructor copia que se hace
     * internamente en el pop lance una excepción.
     */
    s.peek().close();
    // El pop espero que falle ya que el constructor copia lanzará una excepción por file descriptor inválido
    EXPECT_THROW(s.pop(), std::runtime_error);
    // Si el método es exception safe strong, el objeto se mantiene inalterado luego de fallar (es decir, el ultimo
    // DummyFile deberia ser el cerrado, con file descriptor -1
    EXPECT_EQ(s.peek(), DummyFile());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}