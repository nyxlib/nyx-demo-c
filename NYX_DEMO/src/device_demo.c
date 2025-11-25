/*--------------------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>

#include "autogen/glue.h"

/*--------------------------------------------------------------------------------------------------------------------*/

typedef enum demo_mode_e
{
    DEMO_MODE_NOISE = 0,
    DEMO_MODE_DELTA = 1,
    DEMO_MODE_COMB = 2,

} demo_mode_t;

/*--------------------------------------------------------------------------------------------------------------------*/

static nyx_onoff_t s_run = NYX_ONOFF_OFF;
static demo_mode_t s_mode = DEMO_MODE_NOISE;

static float s_samp_rate = 2000000.0f;
static float s_frequency = 143050000.0f;
static float s_power = -30.0f;

static unsigned int s_fft_size = 512U;

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _run_run_callback(nyx_dict_t *vector, nyx_dict_t *def, int new_value, int old_value)
{
    s_run = (nyx_onoff_t) new_value;

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _signal_mode_mode_noise_callback(nyx_dict_t *vector, nyx_dict_t *def, int new_value, int old_value)
{
    if(new_value == NYX_ONOFF_ON)
    {
        s_mode = DEMO_MODE_NOISE;
    }

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _signal_mode_mode_delta_callback(nyx_dict_t *vector, nyx_dict_t *def, int new_value, int old_value)
{
    if(new_value == NYX_ONOFF_ON)
    {
        s_mode = DEMO_MODE_DELTA;
    }

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _signal_mode_mode_comb_callback(nyx_dict_t *vector, nyx_dict_t *def, int new_value, int old_value)
{
    if(new_value == NYX_ONOFF_ON)
    {
        s_mode = DEMO_MODE_COMB;
    }

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _signal_params_samp_rate_callback(nyx_dict_t *vector, nyx_dict_t *def, double new_value, double old_value)
{
    s_samp_rate = (float) new_value;

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _signal_params_frequency_callback(nyx_dict_t *vector, nyx_dict_t *def, double new_value, double old_value)
{
    s_frequency = (float) new_value;

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _signal_params_power_callback(nyx_dict_t *vector, nyx_dict_t *def, double new_value, double old_value)
{
    s_power = (float) new_value;

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static bool _fft_params_fft_size_callback(nyx_dict_t *vector, nyx_dict_t *def, unsigned int new_value, unsigned int old_value)
{
    s_fft_size = new_value;

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void gen_noise(float *dst, size_t n, float mean_db)
{
    for(size_t i = 0; i < n; i++)
    {
        float u = (float) rand() / (float) RAND_MAX; // NOLINT(*-msc50-cpp)
        float v = (float) rand() / (float) RAND_MAX; // NOLINT(*-msc50-cpp)

        float w = (u + v - 1.0f) * 6.0f;

        dst[i] = mean_db + w;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void gen_delta(float *dst, size_t n, float mean_db)
{
    gen_noise(dst, n, mean_db);

    dst[n / 2U] += 20.0f;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void gen_comb(float *dst, size_t n, float mean_db)
{
    gen_noise(dst, n, mean_db);

    size_t step = n / 8u;

    for(size_t i = step / 2u; i < n; i += step)
    {
        dst[i] += 20.0f;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

static void timer_stream(__NYX_UNUSED__ void *arg)
{
    if(s_run != NYX_ONOFF_ON)
    {
        return;
    }

    /*----------------------------------------------------------------------------------------------------------------*/

    static float spectrum[4096];

    const size_t n = (s_fft_size <= 4096U) ? (size_t) s_fft_size
                                           : (size_t) 4096U
    ;

    switch(s_mode)
    {
        case DEMO_MODE_NOISE: gen_noise(spectrum, n, s_power); break;
        case DEMO_MODE_DELTA: gen_delta(spectrum, n, s_power); break;
        default:              gen_comb(spectrum, n, s_power); break;
    }

    /*----------------------------------------------------------------------------------------------------------------*/

    size_t sizes[] = {sizeof(s_samp_rate), sizeof(s_frequency), n * sizeof(float)};
    BUFF_t buffs[] = {&s_samp_rate       , &s_frequency       , spectrum         };

    nyx_stream_pub(vector_demo_spectrum, 100, 3, sizes, buffs);

    /*----------------------------------------------------------------------------------------------------------------*/
}

/*--------------------------------------------------------------------------------------------------------------------*/

void device_demo_initialize(nyx_node_t *node)
{
    vector_demo_run_run->base.in_callback._int = _run_run_callback;
    vector_demo_signal_mode_mode_noise->base.in_callback._int = _signal_mode_mode_noise_callback;
    vector_demo_signal_mode_mode_delta->base.in_callback._int = _signal_mode_mode_delta_callback;
    vector_demo_signal_mode_mode_comb->base.in_callback._int = _signal_mode_mode_comb_callback;
    vector_demo_signal_params_samp_rate->base.in_callback._double = _signal_params_samp_rate_callback;
    vector_demo_signal_params_frequency->base.in_callback._double = _signal_params_frequency_callback;
    vector_demo_signal_params_power->base.in_callback._double = _signal_params_power_callback;
    vector_demo_fft_params_fft_size->base.in_callback._uint = _fft_params_fft_size_callback;

    nyx_node_add_timer(node, 50, timer_stream, NULL);
}

/*--------------------------------------------------------------------------------------------------------------------*/

void device_demo_finalize(nyx_node_t *node)
{
    /* DO NOTHING */
}

/*--------------------------------------------------------------------------------------------------------------------*/
