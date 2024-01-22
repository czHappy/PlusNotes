--
-- PostgreSQL database dump
--

-- Dumped from database version 12.16 (Debian 12.16-1.pgdg120+1)
-- Dumped by pg_dump version 14.9 (Ubuntu 14.9-0ubuntu0.22.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: map_localization; Type: TABLE; Schema: public; Owner: root
--

CREATE TABLE public.map_localization (
    localization_status real,
    map_location_lane_l real,
    map_location_lane_s real,
    map_location_lane_id real,
    map_location_road_id real,
    map_location_lane_idx real,
    map_location_status real,
    gnss_localization_status real,
    map_quality real,
    map_quality_reason real,
    vl_result_quality real,
    vl_longitudinal_offset real,
    vl_pitch_offset real,
    vl_yaw_offset real,
    vl_mode real,
    region_map_data_source real,
    ego_behavior real,
    ego_lane_lateral_location real,
    ego_lane_semantic_info real,
    map_curvature real,
    map_pitch real,
    ego_map_curvature real,
    ego_map_pitch real,
    speed_limit real,
    ego_map_lane_width real,
    supported_road_type boolean,
    within_geo_fence boolean,
    map_locate_success boolean,
    road_type real,
    tunnel_ahead real,
    in_lk_allow_area boolean,
    potential_under_overpass boolean,
    abnormal_lane_boundary_ahead_from_map boolean,
    disable_map_near_tunnel boolean,
    lane_type real,
    vehicle_timestamp timestamp without time zone,
    vehicle_name character varying,
    near_service_district boolean,
    near_tuunel boolean,
    near_merge boolean,
    near_split boolean,
    toll_booth_ahead boolean,
    is_on_routing_road boolean
);


ALTER TABLE public.map_localization OWNER TO root;

--
-- PostgreSQL database dump complete
--

