#include "src/xdxf2html.h"

#include <iostream>

int main()
{
	dom d(R"(<k>ovalis</k><br><b>ŏvālis</b>, <b>e</b>, adj. ovo, <i>of</i> or <i>belonging to an ovation</i> (post-class.): ovalis corona murtea est: eā utebantur imperatores qui ovantes urbem introibant, Gell. 5, 6, 20; cf. Fest. p. 195 Mll.</br>)");
	builder b("test");
	std::cout << b.get_html(d.root) << '\n';
}