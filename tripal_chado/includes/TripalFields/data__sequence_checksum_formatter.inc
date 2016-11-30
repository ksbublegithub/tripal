<?php

class data__sequence_checksum_formatter extends TripalFieldFormatter {
  // The default lable for this field.
  public static $label = 'Sequence MD5 checksum';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('data__sequence_checksum');

  // The list of default settings for this formatter.
  public static $settings = array();


 /**
  * @see TripalFieldFormatter::view()
  */
  public function view(&$element, $entity_type, $entity, $langcode, $items, $display) {
    foreach ($items as $delta => $item) {
      $content = key_exists('value', $item) ? $item['value'] : '';
      $element[$delta] = array(
        // We create a render array to produce the desired markup,
        '#type' => 'markup',
        '#markup' => $content,
      );
    }
  }
}