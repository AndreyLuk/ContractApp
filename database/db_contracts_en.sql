-- ********************************************************************
-- Schema: contracts
DROP SCHEMA IF EXISTS contracts CASCADE;
CREATE SCHEMA contracts AUTHORIZATION postgres;
SET search_path TO contracts;
-- ********************************************************************
CREATE TABLE partner
(
  partner_id serial PRIMARY KEY,
  partner_name text NOT NULL,
  inum text
);

CREATE TABLE contract_type
(
  type_id serial PRIMARY KEY,
  type_name varchar(50)
);

CREATE TABLE contract_state
(
  state_id serial PRIMARY KEY,
  state_name varchar(30)
);

CREATE TABLE contract
(
  contract_id serial PRIMARY KEY,
  type_id int NOT NULL REFERENCES contract_type,
  state_id int NOT NULL REFERENCES contract_state,
  partner_id int NOT NULL REFERENCES partner,
  contract_num varchar(50),
  contract_date date,
  start_date date,
  finish_date date,
  vat_rate numeric(4,1) DEFAULT 0,
  contract_sum money DEFAULT 0,
  vat_sum money DEFAULT 0,
  total_sum money DEFAULT 0,
  contract_note text,
  updated_date timestamp DEFAULT now(),
  updated_by varchar(30) DEFAULT user
);

CREATE INDEX idx_contract_fk_type_id ON contract (type_id);
CREATE INDEX idx_contract_fk_state_id ON contract (state_id);
CREATE INDEX idx_contract_fk_partner_id ON contract (partner_id);

-- ********************************************************************
-- create trigger functions
-- ********************************************************************
CREATE FUNCTION trf_updated ()
	RETURNS trigger
	LANGUAGE plpgsql
	VOLATILE 
	CALLED ON NULL INPUT
	SECURITY INVOKER
	COST 100
	AS $$
BEGIN
    NEW.updated_date = CURRENT_TIMESTAMP;
    NEW.updated_by = "current_user"();
    NEW.vat_sum = NEW.contract_sum * ( NEW.vat_rate / 100 );
    NEW.total_sum = NEW.contract_sum + NEW.vat_sum;
    RETURN NEW;
END 
$$;

CREATE FUNCTION trf_inserted ()
	RETURNS trigger
	LANGUAGE plpgsql
	VOLATILE 
	CALLED ON NULL INPUT
	SECURITY INVOKER
	COST 100
	AS $$
BEGIN
    NEW.vat_sum = NEW.contract_sum * ( NEW.vat_rate / 100 );
    NEW.total_sum = NEW.contract_sum + NEW.vat_sum;
    RETURN NEW;
END 
$$;

-- ********************************************************************
-- create triggers
-- ********************************************************************
CREATE TRIGGER tr_updated
	BEFORE UPDATE
	ON contract
	FOR EACH ROW
	EXECUTE PROCEDURE trf_updated();

CREATE TRIGGER tr_inserted
	BEFORE INSERT
	ON contract
	FOR EACH ROW
	EXECUTE PROCEDURE trf_inserted();

-- ********************************************************************
-- Inserting test data into contracts schema.

INSERT INTO partner (partner_name, inum)
  VALUES ('Company 1', '7701001001'),
  ('Company 2', '7701001002'),
  ('Company 3', '7701001003'),
  ('Company 4', '7701001004'),
  ('Company 5', '7701001005'),
  ('Company 6', '7701001006'),
  ('Company 7', '7701001007'),
  ('Company 8', '7701001008'),
  ('Company 9', '7701001009'),
  ('Company 10', '7701001010'),
  ('Company 11', '7701001011'),
  ('Company 12', '7701001012'),
  ('Company 13', '7701001013'),
  ('Company 14', '7701001014'),
  ('Company 15', '7701001015'),
  ('Company 16', '7701001016'),
  ('Company 17', '7701001017'),
  ('Company 18', '7701001018'),
  ('Company 19', '7701001019'),
  ('Company 20', '7701001020');

INSERT INTO contract_type (type_name)
  VALUES ('Sales'),
  ('Buying'),
  ('Banking'),
  ('Loan'),
  ('Service'),
  ('Insurance'),
  ('Debit'),
  ('Credit'),
  ('Tax'),
  ('Rent'),
  ('Other');

INSERT INTO contract_state (state_name)
  VALUES ('Unknown'),
  ('Canceled'),
  ('Closed'),
  ('Deleted'),
  ('Frozen'),
  ('Blocked'),
  ('Opened');

INSERT INTO contract (type_id, state_id, partner_id, contract_num,
    contract_date, start_date, finish_date, vat_rate, contract_sum)
  VALUES 
   (1, 7, 1, '01', '2023-01-11',  '2023-01-12', '2023-12-31', 10, 1000),
   (1, 7, 2, '02', '2023-01-11',  '2023-01-22', '2023-12-21', 0, 2000),
   (1, 7, 4, '03', '2023-01-11',  '2023-01-22', '2023-12-22', 20, 3000),
   (2, 1, 5, '04', '2023-01-11',  '2023-01-22', '2023-12-31', 18, 4000),
   (2, 7, 6, '05', '2023-01-11',  '2023-01-22', '2023-12-31', 10, 5000),
   (2, 7, 6, '06', '2023-01-12',  '2023-01-22', '2023-11-30', 0, 21000),
   (1, 7, 2, '07', '2023-01-12',  '2023-01-20', '2023-11-30', 10, 12000),
   (3, 2, 7, '08', '2023-01-12',  '2023-01-21', '2023-11-15', 18, 11000),
   (3, 7, 1, '09', '2023-01-13',  '2023-02-01', '2023-08-15', 20, 12000),
   (1, 3, 1, '10', '2023-01-13',  '2023-02-01', '2023-08-15', 10, 13000),
   (1, 7, 8, '11', '2023-01-13',  '2023-02-02', '2023-08-15', 0, 14000),
   (4, 7, 8, '12', '2023-01-14',  '2023-02-02', '2023-04-16', 0, 15000),
   (4, 7, 4, '13', '2023-01-14',  '2023-02-02', '2023-04-16', 10, 16000),
   (1, 4, 3, '14', '2023-01-14',  '2023-02-02', '2023-09-14', 18, 17000),
   (5, 7, 4, '15', '2023-01-14',  '2023-02-02', '2023-09-14', 18, 18000),
   (5, 7, 5, '16', '2023-01-14',  '2023-02-02', '2023-11-14', 0, 19000),
   (1, 7, 2, '17', '2023-01-14',  '2023-02-10', '2023-11-15', 10, 20000),
   (6, 5, 5, '18', '2023-01-14',  '2023-02-10', '2023-11-15', 0, 22000),
   (6, 7, 5, '19', '2023-01-15',  '2023-02-10', '2023-12-15', 10, 24000),
   (7, 7, 2, '20', '2023-01-15',  '2023-02-10', '2023-12-13', 10, 25000),
   (8, 2, 6, '21', '2023-01-15',  '2023-02-12', '2023-12-13', 10, 56000),
   (9, 7, 6, '22', '2023-01-15',  '2023-02-12', '2023-12-13', 20, 15000),
   (1, 7, 3, '23', '2023-01-15',  '2023-02-12', '2023-11-13', 18, 56000),
   (1, 7, 3, '24', '2023-01-16',  '2023-02-12', '2023-11-13', 0, 1000);
