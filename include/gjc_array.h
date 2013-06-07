#include<vector>
#include<map>
#include"fib_array.h"
#include"value_reader.h"

namespace yunomi{
	template <class T> class convert_reader : public value_reader<size_t>{
	public:
		convert_reader(
				value_reader<T> &origvr,
				std::map<T, size_t> &valuemap)
			: origvr(origvr), valuemap(valuemap){
		}

		virtual size_t size(){
			return origvr.size();
		}
		virtual size_t pop_front(){
			T origvalue = origvr.pop_front();
			return valuemap[origvalue]+1;
		}

	  virtual void move_head(){
			origvr.move_head();
		}

	private:
		value_reader<T> &origvr;
		std::map<T, size_t> &valuemap;
	};

	template <class T> class gjc_array{
	public:
		gjc_array(value_reader<T> &vr){
			size_t size = vr.size();

			std::map<T, size_t> valuemap;
			for(size_t i=0; i < size; i++){
				T value = vr.pop_front();
				if(valuemap.find(value)==valuemap.end()){
					valuemap[value]=1;
				}else{
					valuemap[value] = valuemap[value]+1;
				}
			}
			vr.move_head();

			std::vector< std::pair<size_t, T> > rank;
			typename std::map<T, size_t>::iterator it;
			it = valuemap.begin();

			rank.reserve(size);
			while(it != valuemap.end()){
				T value = (*it).first;
				size_t count = (*it).second;

				rank.push_back( std::pair<size_t, T>(count, value) );

				++it;
			}
			valuemap.clear();

			std::sort(rank.begin(), rank.end(), std::greater< std::pair<size_t, T> >() );

			values_size = rank.size();
			values = new T[values_size];
			for(size_t i = 0; i < values_size; i++){
				values[i] = rank[i].second;
				valuemap[rank[i].second] = i;
			}
			rank.clear();

			convert_reader<T> cvr(vr, valuemap);
			array = new fib_array<size_t>(cvr);
		}

		gjc_array(FILE *fp){
			fread(&values_size, sizeof(size_t), 1, fp);
			values = new T[values_size];
			fread(values, sizeof(T), values_size, fp);
			array->dump(fp);
		}
		
		T operator[](size_t i){
			size_t rank = (*array)[i]-1;
			return values[rank];
		}

		void dump(FILE *fp){
			fwrite(&values_size, sizeof(size_t), 1, fp);
			fwrite(values, sizeof(T), values_size, fp);
			array->dump(fp);
		}

	private:
		size_t values_size;
		T *values;
		fib_array<size_t> *array;
	};
}

