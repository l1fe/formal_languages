public class MyClass {
	public static void main(String[] args) {
		int x;
		x = 5;

		if (true) {
			StaticJavaLib.println(x);
			if (true) {
				StaticJavaLib.println(x + 1);
				if (true) {
					StaticJavaLib.println(x + 2);
				} else {
					StaticJavaLib.println(777);
				}
			} else {
				StaticJavaLib.println(666);
			}
		} else {
			StaticJavaLib.println(555);
		}
	}
}
