#include <chrono>

using namespace std;
using namespace std::chrono;

class LinuxTimeUtils {
public:
	static void ServerRequest();
	static long GetDeltaTime();
};
