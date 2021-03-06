#ifndef LXSEXRFWDFGPBGTBCVLETEQBXOVKGTYUGRXIKJXTJRRGAHTKJIXTTXGQRBFPHUYJUOXTFMGRL
#define LXSEXRFWDFGPBGTBCVLETEQBXOVKGTYUGRXIKJXTJRRGAHTKJIXTTXGQRBFPHUYJUOXTFMGRL

#include <f/pattern/pattern.hpp>
#include <f/cuda_rotated_pattern/cuda_rotated_pattern_config.hpp>
#include <f/cuda_rotated_pattern/cuda_rotated_pattern_data.hpp>
#include <f/host/cuda/cuda.hpp>
#include <f/matrix/matrix.hpp>
#include <f/wave_length/wave_length.hpp>

#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <iomanip>

void make_rotation_diag( double lambda, double kx_scaler, double kx_offset, double ky_scaler, double ky_offset, double rotation, double* tilt, double* gvec, double* cuda_diag, unsigned long const tilt_size, unsigned long max_dim );
void make_pattern_intensity_diff( double* cuda_ug, unsigned long* cuda_ar, double* cuda_diag, double thickness, unsigned long* cuda_dim, double* cuda_I_exp, double* cuda_I_diff, unsigned long column_index, double2* cuda_cache, unsigned long tilt_size, unsigned long max_dim, double ac, double dc );

namespace f
{

    struct cuda_pattern
    {
        typedef double                  value_type;
        typedef unsigned long int       size_type;

        cuda_pattern_config             config;
        cuda_pattern_data               data;

        //thickness, kx_f, kx_o, ky_f, ky_o, rotation
        size_type unknowns()
        {
			return config.ug_size*2 + 6;
        }

        cuda_pattern( pattern<value_type> const& pat, int device_id = 0 ) : config{ make_cuda_pattern_config( pat, device_id ) }, data{ config }
        {
            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            import( pat );
        }

        cuda_pattern( pattern<value_type> const& pat, matrix<double> const& gvec, matrix<double> const& tilt, matrix<double> const& ug, int device_id = 0 ) : config{ make_cuda_pattern_config( pat, device_id ) }, data{ config } 
        {
            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            import( pat );
            import_gvec( gvec );
            import_tilt( tilt );
            import_ug( ug );
        }

		std::function<value_type(value_type*)> make_abs_function()
		{
			return [this](value_type* x)
			{
				return (*this).abs_residual(x);
			};
		}

		std::function<value_type(value_type*)> make_merit_function()
		{
			return [this](value_type* x)
			{
				return (*this).square_residual(x);
			};
		}

        //value_type square_residual( value_type* ug, value_type thickness )
        value_type square_residual( value_type* x )
        {
            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            //update_I_diff(ug, thickness);
            update_rotation( x );

            value_type residual;
            cublasHandle_t handle;
            cublas_assert( cublasCreate_v2(&handle) );
            cublas_assert( cublasDdot_v2( handle, static_cast<int>(config.max_dim*config.tilt_size), data.I_diff, 1, data.I_diff, 1, &residual ) );
            cublas_assert( cublasDestroy_v2(handle) );
            return residual;
        }

        value_type abs_residual( value_type* x )
        {
            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            update_rotation( x );

            value_type residual;
            cublasHandle_t handle;
            cublas_assert( cublasCreate_v2(&handle) );
            //cublas_assert( cublasDnrm2_v2( handle, config.max_dim*config.tilt_size, data.I_diff, 1, &residual ) );
            cublas_assert( cublasDasum_v2( handle, static_cast<int>(config.max_dim*config.tilt_size), data.I_diff, 1, &residual ) );
            cublas_assert( cublasDestroy_v2(handle) );

            return residual;
        }

        void import( pattern<value_type> const& pat )
        {
            std::vector<size_type> v_dims;

            for ( size_type index = 0; index != config.tilt_size; ++index )
            {
                //std::cerr << "Importing pattern index " << index << "\n";
                //std::cerr << "pat.diag size is " << pat.diag.size() << "\n";
                v_dims.push_back( pat.diag[index].size() );
                //ar
                size_type const ar_offset = index * config.max_dim * config.max_dim;
                size_type const ar_size = pat.ar[index].size() * sizeof( size_type );
                cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.ar + ar_offset), reinterpret_cast<const void*>(pat.ar[index].data()), ar_size, cudaMemcpyHostToDevice ) );
                //diag
                size_type const diag_offset = index * config.max_dim;
                size_type const diag_size = pat.diag[index].size() * sizeof( value_type );
                cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.diag + diag_offset), reinterpret_cast<const void*>(pat.diag[index].data()), diag_size, cudaMemcpyHostToDevice ) );
                //intensity
                size_type const I_exp_offset = index * config.max_dim;
                size_type const I_exp_size = pat.intensity[index].size() * sizeof( value_type );
                cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.I_exp + I_exp_offset), reinterpret_cast<const void*>(pat.intensity[index].data()), I_exp_size, cudaMemcpyHostToDevice ) );
            }

            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.dim), reinterpret_cast<const void*>(v_dims.data()), sizeof(size_type) * v_dims.size(), cudaMemcpyHostToDevice ) );
        }

        void import_gvec( matrix<double> const& cpu_gvec )
        {
            assert( 2 == cpu_gvec.col() );
            assert( config.ug_size == cpu_gvec.row() );
            size_type const gvec_size = config.ug_size * 2 * sizeof(value_type);
            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.gvec), reinterpret_cast<const void*>(cpu_gvec.data()), gvec_size, cudaMemcpyHostToDevice ) );
        }

        void import_tilt( matrix<double> const& cpu_tilt )
        {
            assert( cpu_tilt.row() == config.tilt_size );
            assert( cpu_tilt.col() == 2 );
            size_type const tilt_size = config.tilt_size * 2 * sizeof(value_type);
            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.tilt), reinterpret_cast<const void*>(cpu_tilt.data()), tilt_size, cudaMemcpyHostToDevice ) );
        }

        void import_ug( value_type* ug )
        {
            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.ug), reinterpret_cast<const void*>(ug), config.ug_size*sizeof(value_type)*2, cudaMemcpyHostToDevice ) );
        }

        void import_ug( matrix<double> const& ug )
        {
            assert( ug.col() == 2 );
            assert( ug.row() == config.ug_size );
            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.ug), reinterpret_cast<const void*>(ug.data()), config.ug_size*sizeof(value_type)*2, cudaMemcpyHostToDevice ) );
        }

        void update_rotation( value_type* x )
        {
			/*
            value_type const rotation = x[0];
            value_type const kx_scaler = x[1];
            value_type const kx_offset = x[2];
            value_type const ky_scaler = x[3];
            value_type const ky_offset = x[4];
            value_type const I_ac = x[5];
            value_type const I_dc = x[6];
            value_type const thickness = x[7];
			*/
			unsigned long const offset = config.ug_size * 2;
			value_type const thickness = x[offset];
            value_type const I_ac = x[0];
            value_type const I_dc = x[1];
            value_type const kx_scaler = x[offset+1];
            value_type const kx_offset = x[offset+2];
            value_type const ky_scaler = x[offset+3];
            value_type const ky_offset = x[offset+4];
            value_type const rotation =  x[offset+5];

            config.thickness = thickness;

			update_ug( x );

            make_rotation_diag( wave_length(120.0), kx_scaler, kx_offset, ky_scaler, ky_offset, rotation, data.tilt, data.gvec, data.diag, config.tilt_size, config.max_dim );
            make_pattern_intensity_diff( data.ug, data.ar, data.diag, thickness, data.dim, data.I_exp, data.I_diff, config.column_index, data.cache, config.tilt_size, config.max_dim, I_ac, I_dc );
        }

		void update_ug( value_type* ug )
		{
			cuda_assert( cudaMemcpy( reinterpret_cast<void*>(data.ug), reinterpret_cast<const void*>(ug), config.ug_size*sizeof(value_type)*2, cudaMemcpyHostToDevice  )  );
		}
/*
        void update_I_diff( value_type* ug, value_type thickness )
        {
            update_thickness( thickness );
            update_ug( ug );

            value_type const kx_scaler = ug[config.ug_size*2+1];
            value_type const kx_offset = ug[config.ug_size*2+2];
            value_type const ky_scaler = ug[config.ug_size*2+3];
            value_type const ky_offset = ug[config.ug_size*2+4];
            value_type const rotation =  ug[config.ug_size*2+5];

            make_rotation_diag( wave_length(120.0), kx_scaler, kx_offset, ky_scaler, ky_offset, rotation, data.tilt, data.gvec, data.diag, config.tilt_size, config.max_dim );

            make_pattern_intensity_diff( data.ug, data.ar, data.diag, config.thickness, data.dim, data.I_exp, data.I_diff, config.column_index, data.cache, config.tilt_size, config.max_dim  );
        }

        void update_thickness( value_type thickness )
        {
            config.thickness = thickness;
        }
*/

        void dump_ug( char const* const file_name )
        {
            matrix<double> ug{ 1, config.ug_size*2 };

            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            device_to_host_copy_n( data.ug, ug.size(), &ug[0][0] );

            ug.save_as( file_name );
        }

        void dump_a()
        {
            matrix<double> a{ config.max_dim, config.max_dim+config.max_dim };

            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            device_to_host_copy_n( data.cache, a.size(), &a[0][0] );

            std::cout << "a is \n";
            std::cout << a << "\n";
        }

        void dump_I_diff()
        {
            matrix<double> Idiff{ config.tilt_size, config.max_dim };

            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            device_to_host_copy( data.I_diff, data.I_diff + Idiff.size(), &Idiff[0][0] );

            std::cout.precision( 15 );
            std::cout << Idiff << "\n";

            std::cout << "\nsquare residual is " << std::inner_product( Idiff.begin(), Idiff.end(), Idiff.begin(), 0.0 ) << "\n";

        }

        void dump_I_exp()
        {
            matrix<double> Iexp{ config.tilt_size, config.max_dim };

            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(Iexp.data()), reinterpret_cast<const void*>(data.I_exp), sizeof(value_type) * Iexp.size(), cudaMemcpyDeviceToHost ) );

            std::copy( Iexp.row_begin(0), Iexp.row_end(), std::ostream_iterator<double>( std::cout, "\t" ) );
            std::cout << "\n";
        }

        void dump_diag( char const* const file_name )
        {
            matrix<double> Pdiag{ config.tilt_size, config.max_dim };
            //matrix<double> Pdiag{ config.max_dim, config.tilt_size };

            int current_id;
            cuda_assert( cudaGetDevice(&current_id) );
            if ( current_id != config.device_id ) cuda_assert( cudaSetDevice( config.device_id ) );

            cuda_assert( cudaMemcpy( reinterpret_cast<void*>(Pdiag.data()), reinterpret_cast<const void*>(data.diag), sizeof(value_type) * Pdiag.size(), cudaMemcpyDeviceToHost ) );

            Pdiag.save_as( file_name );
        }

    };//struct cuda_pattern

}

#endif//LXSEXRFWDFGPBGTBCVLETEQBXOVKGTYUGRXIKJXTJRRGAHTKJIXTTXGQRBFPHUYJUOXTFMGRL

