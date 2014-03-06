public class MainClass {
	public static void main(String[] arguments) {
		int i;

		for (i = 0; i < 5; i = i + 1) {
			StaticJavaLib.println(i);
		}

		while (i < 10) {
			StaticJavaLib.println(i * i);
			i = i + 1;
		}
	
		if (i != 10) {
			StaticJavaLib.println(666);
		} else {
			StaticJavaLib.println(-5);
		}
	}
}
