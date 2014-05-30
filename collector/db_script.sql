--
-- Table tweets creation
--
CREATE TABLE tweets (
    id bigint NOT NULL,
    profile character varying(50) NOT NULL,
    name character varying(50) DEFAULT NULL::character varying,
    tweet text NOT NULL,
    created_at timestamp without time zone DEFAULT now(),
    location character varying(100) DEFAULT NULL::character varying
);


ALTER TABLE public.tweets OWNER TO postgres;

--
-- Name: tweets_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tweets_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.tweets_id_seq OWNER TO postgres;

--
-- Name: tweets_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tweets_id_seq OWNED BY tweets.id;


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tweets ALTER COLUMN id SET DEFAULT nextval('tweets_id_seq'::regclass);
