#include <boost/python/class.hpp>
#include <boost/mpl/vector.hpp>

#include <moveit/task_constructor/properties.h>

namespace moveit {
namespace python {

void export_properties();

namespace properties {

/** Extension for boost::python::class_ to allow convienient definition of properties
 *
 * New method add_property<PropertyType>(const char* name) adds a property getter.
 */

template <
    class W // class being wrapped
    , class X1 = ::boost::python::detail::not_specified
    , class X2 = ::boost::python::detail::not_specified
    , class X3 = ::boost::python::detail::not_specified
    >
class class_ : public boost::python::class_<W, X1, X2, X3>
{
public:
	typedef class_<W,X1,X2,X3> self;
	// forward all constructors
	using boost::python::class_<W, X1, X2, X3>::class_;

	template <typename PropertyType>
	self& add_property(const char* name, const char* docstr = 0) {
		auto getter = [name](const W& me) {
			const moveit::task_constructor::PropertyMap& props = me.properties();
			return props.get<PropertyType>(name);
		};
		auto setter = [name](W& me, const PropertyType& value) {
			moveit::task_constructor::PropertyMap& props = me.properties();
			props.setCurrent(name, boost::any(value));
		};

		boost::python::class_<W, X1, X2, X3>::add_property
		      (name, boost::python::make_function
		       (getter, boost::python::default_call_policies(),
		        boost::mpl::vector<PropertyType, const W&>()),
		       boost::python::make_function
		       (setter, boost::python::default_call_policies(),
		        boost::mpl::vector<void, W&, const PropertyType&>()),
		       docstr);
		return *this;
	}
};

} } }