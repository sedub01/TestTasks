
public class AddParameters {
    public static void main(String[] args) {
        int value = 0x5FABFF01;

        //Первый доп. параметр из второго байта
        int firstParameter = (value >> 8) & 0xFF; //0xFF == 255

        //Второй доп. параметр: инверсия 7-го бита исходного значения
        int seventhBit = (value >> 6) & 0x1;
        int secondParameter = seventhBit ^ 1;

        //Третий доп. параметр: зеркальное отображение 17-20 го бита
        int bit17to20 = (value >> 16) & 0x0F;
        int thirdParameter = ((bit17to20 & 0x08) >> 3) | ((bit17to20 & 0x04) >> 1) |
                             ((bit17to20 & 0x02) << 1) | ((bit17to20 & 0x01) << 3);

        System.out.println("Первый дополнительный параметр: " + firstParameter);
        System.out.println("Второй дополнительный параметр: " + secondParameter);
        System.out.println("Третий дополнительный параметр: " + thirdParameter);
    }    
}
