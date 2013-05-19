#ifndef YUNOMI_VALUE_READER_H_
#define YUNOMI_VALUE_READER_H_

namespace yunomi{
	template <class T> class value_reader{
	public:
		value_reader(){}
		virtual ~value_reader(){}

		virtual size_t size() = 0;
		virtual T pop_front() = 0;
		virtual void move_head() = 0;
	private:
	};
}

#endif //YUNOMI_VALUE_READER_H_

