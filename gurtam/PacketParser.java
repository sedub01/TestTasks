
public class PacketParser {
    private final static int MAX_SD_FIELDS = 10;
    
    public static void main(String[] args) {
        String packet1 = "#SD#04012011;135515;5544.6025;N;03739.6834;E;35;215;110;7\r\n";
        String packet2 = "#M#груз доставлен\r\n";

        parsePacket(packet1);
        parsePacket(packet2);
    }

    private static void parsePacket(String packet) {
        String[] parts = packet.split("#");
        if (parts.length != 3 || parts[1].length() > 2 || parts[1].length() == 0 || !parts[2].endsWith("\r\n")) {
            System.out.println("Invalid packet format");
            return;
        }

        String packetType = parts[1];
        String data = parts[2].trim();
        switch (packetType) {
            case "SD":
                parseSDPacket(data);
                break;
            case "M":
                parseMPacket(data);
                break;
            default:
                System.out.println("Unknown packet type");
        }
    }

    private static void parseSDPacket(String data) {
        String[] fields = data.split(";");
        if (fields.length != MAX_SD_FIELDS) {
            System.out.println("Invalid SD packet format");
            return;
        }

        String date = fields[0];
        String time = fields[1];
        String lat = fields[2] + fields[3];
        String lon = fields[4] + fields[5];
        int speed = Integer.parseInt(fields[6]);
        int course = Integer.parseInt(fields[7]);
        int height = Integer.parseInt(fields[8]);
        int sats = Integer.parseInt(fields[9]);

        System.out.println("Date: " + date);
        System.out.println("Time: " + time);
        System.out.println("Latitude: " + lat);
        System.out.println("Longitude: " + lon);
        System.out.println("Speed: " + speed);
        System.out.println("Course: " + course);
        System.out.println("Height: " + height);
        System.out.println("Satellites: " + sats);
    }

    private static void parseMPacket(String data) {
        System.out.println("Message: " + data);
    }
}
