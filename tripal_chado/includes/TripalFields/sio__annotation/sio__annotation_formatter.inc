<?php

class sio__annotation_formatter extends ChadoFieldFormatter {
  // The default lable for this field.
  public static $default_label = 'Chado Annotation';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('chado_linker__cvterm');


  /**
   *
   * @see TripalFieldFormatter::settingsForm()
   */
  public function settingsForm($view_mode, $form, &$form_state) {
    $headers = array('Term', 'Definition', 'Is Not', 'Reference');
    $rows = array();

    $chado_table = $this->instance['settings']['chado_table'];
    foreach ($items as $delta => $item) {
      if ($item['chado-' . $chado_table . '__cvterm_id']) {
        $cvterm = chado_generate_var('cvterm', array('cvterm_id' => $item['chado-' . $chado_table . '__cvterm_id']));
        $dbxref = $cvterm->dbxref_id;

        // Build the accession.
        $accession = $dbxref->db_id->name . ':' . $dbxref->accession;
        if ($dbxref->db_id->urlprefix) {
          $accession = l($accession, tripal_get_dbxref_url($dbxref), array('attributes' => array('target' => '_blank')));
        }

        // Build the publication reference.
        $pub_ref = '';
        $pub_id = $item['chado-' . $chado_table . '__pub_id'];
        if ($pub_id) {
          $pub = chado_generate_var('pub', array('pub_id' => $pub_id));
          $pub_ref = $pub->title;
        }
        $rows[] = array(
          $accession,
          $cvterm->definition,
          $item['chado-' . $chado_table . '__is_not'] ? 'Yes' : '',
          '',
        );
      }
    }

    // the $table array contains the headers and rows array as well as other
    // options for controlling the display of the table.  Additional
    // documentation can be found here:
    // https://api.drupal.org/api/drupal/includes%21theme.inc/function/theme_table/7
    $table = array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array(
        'id' => "$chado_table-table-terms",
        'class' => 'tripal-data-table'
      ),
      'caption' => '',
      'sticky' => FALSE,
      'colgroups' => array(),
      'empty' => 'There are no annotations of this type',
    );

    if (count($items) > 0) {
      $element[0] = array(
        '#type' => 'markup',
        '#markup' => theme_table($table),
      );
    }
  }

  /**
   *
   * @see TripalFieldFormatter::view()
   */
  public function view(&$element, $entity_type, $entity, $langcode, $items, $display) {
    $headers = array('Term', 'Definition');
    $rows = array();

    $field_table = $this->instance['settings']['chado_table'];
    $schema = chado_get_schema($field_table);

    if (array_key_exists('is_not', $schema['fields'])) {
      $headers[] = 'Negates';
    }

    $vocabulary_term = tripal_get_chado_semweb_term('cvterm', 'cv_id');
    $accession_term = tripal_get_chado_semweb_term('dbxref', 'accession');
    $definition_term = tripal_get_chado_semweb_term('cvterm', 'definition');
    if (array_key_exists('is_not', $schema['fields'])) {
      $negation_term = tripal_get_chado_semweb_term($field_table, 'is_not');
    }

    $chado_table = $this->instance['settings']['chado_table'];
    foreach ($items as $delta => $item) {

      if ($item['chado-' . $chado_table . '__cvterm_id']) {
        $cvterm = chado_generate_var('cvterm', array('cvterm_id' => $item['chado-' . $chado_table . '__cvterm_id']));
        $dbxref = $cvterm->dbxref_id;

        // Build the accession.
        $accession = $dbxref->db_id->name . ':' . $dbxref->accession;
        if ($dbxref->db_id->urlprefix) {
          $accession = l($accession, tripal_get_dbxref_url($dbxref), array('attributes' => array('target' => '_blank')));
        }

        $row = array(
          $item['value'][$vocabulary_term] . ':' . $item['value'][$accession_term],
          $item['value'][$definition_term],
        );

        if (array_key_exists('is_not', $schema['fields'])) {
          $row[] = $item['value'][$negation_term];
        }

        $rows[] = $row;
      }
    }

    // Theme the results in a talbe.
    $caption = 'This record is associated with the following annotations.';
    if (array_key_exists('is_not', $schema['fields'])) {
      $caption .= 'The "Negates" column indicates if the term does NOT apply.';
    }
    $table = array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array(
        'id' => "$chado_table-table-terms",
        'class' => 'tripal-data-table'
      ),
      'caption' => $caption,
      'sticky' => FALSE,
      'colgroups' => array(),
      'empty' => 'There are no annotations of this type',
    );

    if (count($items) > 0) {
      $element[0] = array(
        '#type' => 'markup',
        '#markup' => theme_table($table),
      );
    }
  }
}