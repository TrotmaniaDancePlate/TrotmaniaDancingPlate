package serial.keyboard

import io.micronaut.runtime.Micronaut

object Application {

    @JvmStatic
    fun main(args: Array<String>) {
        Micronaut.build()
                .packages("serial.keyboard")
                .mainClass(Application.javaClass)
                .start()
    }
}