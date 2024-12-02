void nrf_log_callback(nrf_log_t *p_nrf_log)
{
    char buf[200] = {0};
    char *start = buf;
    char *end = buf + sizeof(buf);
    uint8_t len = p_nrf_log->m_psdu_len > 30? 30: p_nrf_log->m_psdu_len;
    uint8_t i;
    uint8_t *a;
    uint8_t *b;

    if (p_nrf_log->m_log_type == LOG_TYPE_RX_MP)
    {
        if (p_nrf_log->m_dst_ext_addr_check)
        {
            a = p_nrf_log->m_dst_ext_addr;
            b = p_nrf_log->m_local_ext_addr;
            otPlatLog(OT_LOG_LEVEL_CRIT, OT_LOG_REGION_MAC, "addr[oft:%u,dst:%02x%02x%02x%02x,local:%02x%02x%02x%02x]",
                      p_nrf_log->m_dst_addr_offset, a[0], a[1], a[2], a[3], b[0],b[1], b[2],b[3]);
        }

        if (p_nrf_log->m_dst_panid_check)
        {
            a = p_nrf_log->m_dst_panid;
            b = p_nrf_log->m_local_panid;
            otPlatLog(OT_LOG_LEVEL_CRIT, OT_LOG_REGION_MAC, "panid[oft:%u,dst:%02x%02x,local:%02x%02x]",
                      p_nrf_log->m_dst_panid_offset, a[0], a[1], b[0],b[1]);
        }

        for (i = 0; i < len; i++)
        {
            start += snprintf(start, end - start, "%02x ", p_nrf_log->m_psdu[i]);
        }

        otPlatLog(OT_LOG_LEVEL_CRIT, OT_LOG_REGION_MAC, "mp:%u, frame:%s", p_nrf_log->m_is_mp ,buf);
    }
    else if (p_nrf_log->m_log_type == LOG_TYPE_RX)
    {
        for (i = 0; i < p_nrf_log->m_num_core_logs; i++)
        {
            start += snprintf(start, end - start, "TP%u, ", p_nrf_log->m_core_logs[i]);
        }

        for (i = 0; i < len; i++)
        {
            start += snprintf(start, end - start, "%02x ", p_nrf_log->m_psdu[i]);
        }

        otPlatLog(OT_LOG_LEVEL_CRIT, OT_LOG_REGION_MAC, "Rx(%s):(%u,%u) %s", p_nrf_log->m_is_mp? "MP": "F",
            p_nrf_log->m_prev_level, p_nrf_log->m_parse_level, buf);
    }
}

static int nrf5_init(const struct device *dev)
{
    nrf_802154_core_set_log_callback(nrf_log_callback);
    return 0;
}

